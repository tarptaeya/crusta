#include "bookmarks.h"
#include "tabwidget.h"
#include "tab.h"
#include "toolbar.h"
#include "utils.h"
#include "webview.h"

#include <QMenu>
#include <QUrl>
#include <QWebEngineHistory>

ToolBar::ToolBar(QWidget *parent)
    : QToolBar (parent)
{
    m_backButton = new QToolButton;
    m_forwardButton = new QToolButton;
    m_refreshButton = new QToolButton;
    m_homeButton = new QToolButton;
    m_downloadButton = new QToolButton;
    m_menuButton = new QToolButton;
    m_addressBar = new QLineEdit;
    m_backMenu = new QMenu;
    m_forwardMenu = new QMenu;

    m_backButton->setIcon(QIcon::fromTheme(QStringLiteral("go-previous")));
    m_forwardButton->setIcon(QIcon::fromTheme(QStringLiteral("go-next")));
    m_refreshButton->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    m_homeButton->setIcon(QIcon::fromTheme(QStringLiteral("go-home")));
    m_downloadButton->setIcon(QIcon::fromTheme(QStringLiteral("edit-download")));
    m_menuButton->setIcon(QIcon::fromTheme(QStringLiteral("application-menu")));

    m_backButton->setPopupMode(QToolButton::DelayedPopup);
    m_forwardButton->setPopupMode(QToolButton::DelayedPopup);
    m_menuButton->setPopupMode(QToolButton::InstantPopup);

    m_backButton->setMenu(m_backMenu);
    m_forwardButton->setMenu(m_forwardMenu);

    addWidget(m_backButton);
    addWidget(m_forwardButton);
    addWidget(m_refreshButton);
    addWidget(m_homeButton);
    addWidget(m_addressBar);
    addWidget(m_downloadButton);
    addSeparator();
    addWidget(m_menuButton);

    setupAddressBar();

    setMovable(false);

    connect(m_backButton, &QToolButton::clicked, this, [this] {
        if (!m_tabWidget) {
            return ;
        }

        m_tabWidget->currentTab()->webView()->back();
    });
    connect(m_forwardButton, &QToolButton::clicked, this, [this] {
        if (!m_tabWidget) {
            return ;
        }

        m_tabWidget->currentTab()->webView()->forward();
    });
    connect(m_refreshButton, &QToolButton::clicked, this, [this] {
        if (!m_tabWidget) {
            return ;
        }

        m_tabWidget->currentTab()->webView()->reload();
    });
    connect(m_homeButton, &QToolButton::clicked, this, [this] {
        if (!m_tabWidget) {
            return ;
        }

        m_tabWidget->currentTab()->webView()->loadHome();
    });
}

void ToolBar::setUrl(const QUrl &url)
{
    m_addressBar->setText(url.toDisplayString());
    m_addressBar->setCursorPosition(0);

    if (url.scheme() == QStringLiteral("http")) {
        m_siteAction->setIcon(QIcon::fromTheme(QStringLiteral("globe")));
    } else {
        m_siteAction->setIcon(QIcon::fromTheme(QStringLiteral("document-encrypted")));
    }

    bookmarkChanged(!Bookmarks::isBookmarked(url.toString(QUrl::RemoveFragment)).url.isEmpty());
}

void ToolBar::loadStarted()
{
    m_refreshButton->setIcon(QIcon::fromTheme(QStringLiteral("process-stop")));
}

void ToolBar::loadFinished()
{
    m_refreshButton->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
}

void ToolBar::setHistory(QWebEngineHistory *history)
{
    m_backMenu->clear();
    m_forwardMenu->clear();

    const QList<QWebEngineHistoryItem> backItems = history->backItems(10);
    const QList<QWebEngineHistoryItem> forwardItems = history->forwardItems(10);

    for (const QWebEngineHistoryItem &item : backItems) {
        QAction *action = m_backMenu->addAction(item.title());
        connect(action, &QAction::triggered, this, [this, item] {
            if (!m_tabWidget) {
                return ;
            }

            m_tabWidget->currentTab()->webView()->history()->goToItem(item);
        });
    }

    for (const QWebEngineHistoryItem &item : forwardItems) {
        QAction *action = m_forwardMenu->addAction(item.title());
        connect(action, &QAction::triggered, this, [this, item] {
            if (!m_tabWidget) {
                return ;
            }

            m_tabWidget->currentTab()->webView()->history()->goToItem(item);
        });
    }

    m_backButton->setDisabled(!history->canGoBack());
    m_forwardButton->setDisabled(!history->canGoForward());
}

void ToolBar::setMenu(QMenu *menu)
{
    m_menuButton->setMenu(menu);
}

void ToolBar::setTabWidget(TabWidget *tabWidget)
{
    m_tabWidget = tabWidget;

    connect(m_tabWidget, &TabWidget::urlChanged, this, &ToolBar::setUrl);
    connect(m_tabWidget, &TabWidget::historyChanged, this, &ToolBar::setHistory);
    connect(m_tabWidget, &TabWidget::loadStarted, this, &ToolBar::loadStarted);
    connect(m_tabWidget, &TabWidget::loadFinished, this, &ToolBar::loadFinished);
    connect(m_tabWidget, &TabWidget::popupRequested, this, &ToolBar::showPopup);
}

void ToolBar::bookmarkChanged(bool isBookmarked)
{
    if (isBookmarked) {
        m_bookmarksAction->setIcon(QIcon::fromTheme(QStringLiteral("draw-star-red")));
    } else {
        m_bookmarksAction->setIcon(QIcon::fromTheme(QStringLiteral("draw-star")));
    }
}

void ToolBar::bookmarkChanged(const BookmarkItem &item, bool isBookmarked)
{
    bookmarkChanged(isBookmarked);

    if (isBookmarked) {
        Bookmarks::insertBookmark(item);
    } else {
        Bookmarks::removeBookmark(item.url);
    }
}

void ToolBar::showPopup(QWidget *widget)
{
    m_popupButton->setIcon(m_tabWidget->currentTab()->webView()->icon());
    m_popupButton->setVisible(true);

    QMetaObject::Connection connection = connect(m_tabWidget->currentTab()->webView(), &WebView::iconChanged, [this](const QIcon &icon) { m_popupButton->setIcon(icon); });

    m_addressBar->update();

    connect(widget, &QWidget::destroyed, [this, connection] {
        disconnect(connection);
        m_popupButton->setVisible(false);
    });

    widget->show();

    int x = m_addressBar->x();
    int y = m_addressBar->y() + m_addressBar->height();

    QPoint point = mapToGlobal(QPoint(x, y));

    widget->move(point);
}

void ToolBar::setupAddressBar()
{
    m_siteAction = new QAction;
    m_popupButton = new QAction;
    m_bookmarksAction = new QAction;

    m_siteAction->setIcon(QIcon::fromTheme(QStringLiteral("globe")));
    m_bookmarksAction->setIcon(QIcon::fromTheme(QStringLiteral("draw-star")));

    m_addressBar->addAction(m_siteAction, QLineEdit::LeadingPosition);
    m_addressBar->addAction(m_popupButton, QLineEdit::LeadingPosition);
    m_addressBar->addAction(m_bookmarksAction, QLineEdit::TrailingPosition);

    m_popupButton->setVisible(false);

    connect(m_bookmarksAction, &QAction::triggered, this, [this] {
        if (!m_tabWidget) {
            return ;
        }

        const QIcon icon = m_tabWidget->currentTab()->webView()->icon();
        const QString title = m_tabWidget->currentTab()->webView()->title();
        const QString url = m_tabWidget->currentTab()->webView()->url().toString(QUrl::RemoveFragment);

        BookmarkItem bookmarkItem = Bookmarks::isBookmarked(url);

        BookmarkItem item;
        item.icon = icon;
        item.url = url;
        item.title = bookmarkItem.title.isEmpty() ? title : bookmarkItem.title;
        item.description = bookmarkItem.description;
        item.folder = bookmarkItem.folder;

        if (bookmarkItem.url.isEmpty()) {
            bookmarkChanged(item, true);
        }

        QWidget *widget = Bookmarks::popupWidget(item);
        connect(widget, &QWidget::destroyed, this, [this, item] {
            BookmarkItem bookmarkItem = Bookmarks::isBookmarked(item.url);
            if (bookmarkItem.url.isEmpty()) {
                bookmarkChanged(item, false);
            }
        });
        widget->show();

        int x = m_addressBar->x() + m_addressBar->width() - widget->width();
        int y = m_addressBar->y() + m_addressBar->height();

        QPoint point = mapToGlobal(QPoint(x, y));

        widget->move(point);
    });
}
