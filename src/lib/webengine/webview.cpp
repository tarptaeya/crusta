#include "database.h"
#include "browserwindow.h"
#include "historyitem.h"
#include "mainapplication.h"
#include "tab.h"
#include "tabwidget.h"
#include "webpage.h"
#include "webview.h"

WebView::WebView(QWidget *parent)
    : QWebEngineView (parent)
{
    m_webPage = new WebPage(appManager->webEngineProfile());
    setPage(m_webPage);

    connect(this, &WebView::loadStarted, this, [this] { m_isLoading = true; });
    connect(this, &WebView::loadFinished, this, [this] {
        m_isLoading = false;
        appManager->dataBase()->addHistory(HistoryItem::fromWebEngineHistoryItem(history()->currentItem()));
    });
}

WebView::~WebView()
{
    m_webPage->deleteLater();
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    if (type == QWebEnginePage::WebBrowserTab) {
        Tab *tab = new Tab;
        int index = appManager->currentWindow()->tabWidget()->addTab(tab);
        appManager->currentWindow()->tabWidget()->setCurrentIndex(index);
        return tab->webView();
    } else if (type == QWebEnginePage::WebBrowserBackgroundTab) {
        Tab *tab = new Tab;
        appManager->currentWindow()->tabWidget()->addTab(tab);
        return tab->webView();
    } else if (type == QWebEnginePage::WebBrowserWindow) {
        Tab *tab = new Tab;
        appManager->createWindow(tab);
        return tab->webView();
    } else if (type == QWebEnginePage::WebDialog) {
        if (appManager->settings()->value(QSL("webView/blockPopups"), false).toBool()) {
            return nullptr;
        }

        WebView *webView = new WebView;
        appManager->createDialogWindow(webView);
        return webView;
    }

    return nullptr;
}

bool WebView::isLoading()
{
    return m_isLoading;
}

QString WebView::title() const
{
    QString titleString = QWebEngineView::title();
    if (titleString.isEmpty()) {
        titleString = QSL("Connecting...");
    }

    return titleString;
}
