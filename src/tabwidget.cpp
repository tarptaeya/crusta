#include "browser.h"
#include "tab.h"
#include "tabwidget.h"
#include "webpage.h"
#include "webview.h"

#include <QApplication>
#include <QClipboard>
#include <QMenu>
#include <QSettings>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget (parent)
{
    m_newTabButton = new QToolButton;

    m_newTabButton->setAutoRaise(true);
    m_newTabButton->setObjectName(QStringLiteral("newtabbutton"));
    m_newTabButton->setToolTip(QStringLiteral("Click to add tab"));
    m_newTabButton->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));

    TabBar *tabBar = new TabBar;
    setTabBar(tabBar);
    tabBar->setDocumentMode(true);
    tabBar->setElideMode(Qt::ElideRight);
    tabBar->setExpanding(true);
    tabBar->setMovable(true);
    tabBar->setTabsClosable(true);
    tabBar->setUsesScrollButtons(true);
    tabBar->setContextMenuPolicy(Qt::CustomContextMenu);

    setContentsMargins(0, 0, 0, 0);

    setCornerWidget(m_newTabButton);
    if (Browser::instance()->isPrivate()) {
        m_privateButton = new QToolButton;
        m_privateButton->setAutoRaise(true);
        m_privateButton->setObjectName(QStringLiteral("privatebutton"));
        m_privateButton->setIcon(QIcon::fromTheme(QStringLiteral("view-private")));

        m_privateButton->setAttribute(Qt::WA_TransparentForMouseEvents);

        setCornerWidget(m_privateButton, Qt::TopLeftCorner);
    }

    addTab(new Tab);

    setTabPosition(([] () -> TabWidget::TabPosition {
                        QSettings settings;
                        const QString position = settings.value(QStringLiteral("appearance/tabsposition"), QStringLiteral("Top")).toString();
                        if (position == QStringLiteral("Top")) {
                            return North;
                        }

                        return South;
                    })());

    connect(m_newTabButton, &QToolButton::clicked, this, [this] {
        Tab *tab = new Tab;
        tab->webView()->loadNewTabPage();
        addTab(tab);
    });
    connect(this, &TabWidget::currentChanged, this, [this] (int index) {
        Tab *tab = tabAt(index);
        if (!tab) {
            return ;
        }

        emit urlChanged(tab->webView()->url());
        emit historyChanged(tab->webView()->history());

        tab->webView()->setFocus();

        if (tab->webView()->isLoading()) {
            emit loadStarted();
        } else {
            emit loadFinished();
        }
    });
    connect(tabBar, &QTabBar::customContextMenuRequested, this, &TabWidget::createContextMenu);
    connect(tabBar, &QTabBar::tabCloseRequested, this, [this] (int index) {
        Tab *tab = tabAt(index);
        if (!tab) {
            return;
        }

        removeTab(index);
        tab->deleteLater();

        if (!count()) {
            // TODO: add option to not close window when
            //       last tab is closed
            emit windowCloseRequested();
        }
    });
}

int TabWidget::addTab(Tab *tab, bool isBackground, const QString &label)
{
    WebView *webView = tab->webView();

    connect(webView, &WebView::titleChanged, this, [this, tab] (const QString &title) {
        int index = indexOf(tab);
        setTabText(index, title);
        setTabToolTip(index, title);
    });

    connect(webView, &WebView::iconChanged, this, [this, tab] (const QIcon &icon) {
        int index = indexOf(tab);

        if (tab->webView()->page()->recentlyAudible()) {
            return ;
        }

        setTabIcon(index, icon);
    });

    connect(webView, &WebView::urlChanged, this, [this, tab] (const QUrl &url) {
        int index = indexOf(tab);
        if (index != currentIndex()) {
            return ;
        }
        emit urlChanged(url);
    });

    connect(webView, &WebView::historyChanged, this, [this, tab] (QWebEngineHistory *history) {
        int index = indexOf(tab);
        if (index != currentIndex()) {
            return ;
        }
        emit historyChanged(history);
    });

    connect(webView, &WebView::loadStarted, this, [this, tab] {
        int index = indexOf(tab);
        if (index != currentIndex()) {
            return ;
        }

        emit loadStarted();
    });

    connect(webView, &WebView::loadFinished, this, [this, tab] {
        int index = indexOf(tab);
        if (index != currentIndex()) {
            return ;
        }

        emit loadFinished();
    });

    connect(webView, &WebView::historyItemInserted, this, [this] {
        emit historyItemInserted();
    });

    connect(webView, &WebView::addTabRequested, this, [this] (Tab *tab, bool isBackground) {
        addTab(tab, isBackground);
    });

    connect(webView->page(), &WebPage::recentlyAudibleChanged, this, [this, tab] (bool recentlyAudible) {
        int index = indexOf(tab);
        setTabIcon(index, tab->webView()->page()->isAudioMuted() ? QIcon::fromTheme(QStringLiteral("audio-off")) :
                                                                   recentlyAudible ? QIcon::fromTheme(QStringLiteral("audio-on")) :
                                                                                     tab->webView()->icon());
    });

    connect(webView->page(), &WebPage::popupRequested, [this, tab] (QWidget *widget) {
        int index = indexOf(tab);
        if (index != currentIndex()) {
            return ;
        }

        emit popupRequested(widget);
    });

    int index = QTabWidget::addTab(tab, label);
    if (!isBackground) {
        setCurrentIndex(index);
    }

    return index;
}

Tab *TabWidget::tabAt(int index)
{
    return dynamic_cast<Tab *>(widget(index));
}

Tab *TabWidget::currentTab()
{
    return tabAt(currentIndex());
}

void TabWidget::createContextMenu(const QPoint &pos)
{
    int index = tabBar()->tabAt(pos);
    Tab *tab = tabAt(index);
    if (!tab) {
        return;
    }

    QMenu menu;
    QAction *duplicate = menu.addAction(QStringLiteral("Duplicate tab"));
    QAction *mute = menu.addAction(tab->webView()->page()->isAudioMuted() ? QStringLiteral("Unmute tab") : QStringLiteral("Mute tab"));
    menu.addSeparator();
    QAction *copy = menu.addAction(QStringLiteral("Copy page title"));
    menu.addSeparator();
    QAction *configure = menu.addAction(QStringLiteral("Configure"));

    connect(mute, &QAction::triggered, this, [tab] { tab->webView()->page()->setAudioMuted(!tab->webView()->page()->isAudioMuted()); });
    connect(copy, &QAction::triggered, this, [tab] { qApp->clipboard()->setText(tab->webView()->title()); });

    menu.exec(mapToGlobal(pos));
}

QSize TabBar::tabSizeHint(int index) const
{
    QSize size = QTabBar::tabSizeHint(index);
    int width = this->width() / (count() > 0 ? count() : 1);
    width = width > m_minTabWidth ? width : m_minTabWidth;
    size.setWidth(width);
    return size;
}
