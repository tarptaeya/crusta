#include "common-defs.h"
#include "bookmarkdialog.h"
#include "browserwindow.h"
#include "mainapplication.h"
#include "menu.h"
#include "omnibar.h"
#include "statusbar.h"
#include "tab.h"
#include "tabwidget.h"
#include "toolbar.h"
#include "toolbarbutton.h"
#include "webpage.h"
#include "webview.h"

#include <QWebEngineHistory>
#include <QWebEngineHistoryItem>

Tab::Tab(const QString &address, QWidget *parent)
    : QWidget (parent)
{
    m_vboxLayout = new QVBoxLayout(this);
    m_vboxLayout->setSpacing(0);
    m_vboxLayout->setContentsMargins(0, 0, 0, 0);

    m_toolBar = new ToolBar(this);

    m_webView = new WebView(this);
    if (!address.isEmpty()) {
        m_webView->load(address);
    }
    m_webView->setFocus();

    m_loaderMovie = new QMovie(":/icons/loader.gif");

    m_vboxLayout->addWidget(m_toolBar, 0);
    m_vboxLayout->addWidget(m_webView, 1);
    setLayout(m_vboxLayout);

    connect(m_webView, &WebView::titleChanged, this, [this](const QString &title) {
        if (!m_tabWidget) {
            return;
        }

        m_tabWidget->setTabText(index(), title);
        m_tabWidget->setTabToolTip(index(), title);

        if (m_tabWidget->currentTab() == this) {
            for (BrowserWindow *window : appManager->windows()) {
                if (window->tabWidget() == m_tabWidget) {
                    if (appManager->isPrivate()) {
                        window->setWindowTitle(QSL("%1 - Crusta Private").arg(title));
                    } else {
                        window->setWindowTitle(QSL("%1 - Crusta").arg(title));
                    }
                }
            }
        }
    });

    connect(m_webView, &WebView::iconChanged, this, [this](const QIcon &icon) {
        if (!m_tabWidget) {
            return;
        }

        m_loaderMovie->stop();
        disconnect(m_loaderConnection);
        m_tabWidget->setTabIcon(index(), icon);
    });

    connect(m_webView, &WebView::loadStarted, this, [this] {
        m_toolBar->reloadButton()->setIcon(QIcon::fromTheme(QSL("process-stop")));
        m_loaderMovie->start();

        m_loaderConnection = connect(m_loaderMovie, &QMovie::frameChanged, this, [this] {
            m_tabWidget->setTabIcon(this->index(), m_loaderMovie->currentPixmap());
        });
    });

    connect(m_webView, &WebView::loadFinished, this, [this] {
        m_toolBar->backButton()->setEnabled(m_webView->history()->canGoBack());
        m_toolBar->forwardButton()->setEnabled(m_webView->history()->canGoForward());
        m_toolBar->reloadButton()->setIcon(QIcon::fromTheme(QSL("view-refresh")));

        m_loaderMovie->stop();
        disconnect(m_loaderConnection);
    });

    connect(m_webView, &WebView::loadProgress, this, [this](int progress) {
    });

    connect(m_webView->page(), &WebPage::linkHovered, this, [this](const QUrl &url) {
        for (BrowserWindow *window : appManager->windows()) {
            if (window->tabWidget() == m_tabWidget) {
                window->statusBar()->showMessage(url.toDisplayString());
            }
        }
    });

    connect(m_webView, &WebView::urlChanged, this, [this](const QUrl &url) {
        m_toolBar->omniBar()->update(url.toDisplayString());
    });

    connect(m_toolBar->backButton(), &ToolBarButton::clicked, m_webView, &WebView::back);
    connect(m_toolBar->forwardButton(), &ToolBarButton::clicked, m_webView, &WebView::forward);
    connect(m_toolBar->reloadButton(), &ToolBarButton::clicked, this, [this] {
        if (m_webView->isLoading()) {
            m_webView->stop();
        } else {
            m_webView->reload();
        }
    });
    connect(m_toolBar->homeButton(), &ToolBarButton::clicked, this, [this] {
        m_webView->load(appManager->settings()->value(QSL("webView/homePageUrl"), QSL("crusta://speeddial")).toString());
    });

    connect(m_toolBar->omniBar(), &OmniBar::loadRequested, m_webView, [this](const QString &address){
        m_webView->load(address);
        m_webView->setFocus();
    });

    connect(m_toolBar->omniBar()->bookmarksAction(), &QAction::triggered, this, [this] {
        BookmarkDialog dialog;
        dialog.setAddress(m_webView->url().toDisplayString(QUrl::RemoveQuery));
        dialog.setTitle(m_webView->title());
        dialog.exec();
    });
}

void Tab::setTabWidget(TabWidget *tabWidget)
{
    m_tabWidget = tabWidget;
}

int Tab::index()
{
    if (!m_tabWidget) {
        return -1;
    }

    return m_tabWidget->indexOf(this);
}

QString Tab::title() const
{
    if (!m_webView) {
        return QSL("");
    }
    return m_webView->title();
}

QIcon Tab::icon() const
{
    if (!m_webView) {
        return QIcon();
    }
    return m_webView->icon();
}

WebView *Tab::webView()
{
    return m_webView;
}
