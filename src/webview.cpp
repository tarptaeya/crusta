#include "browser.h"
#include "history.h"
#include "tab.h"
#include "webview.h"
#include "webpage.h"

WebView::WebView(QWidget *parent)
    : QWebEngineView (parent)
{
    m_webPage = new WebPage(Browser::instance()->profile(), this);

    setPage(m_webPage);

    connect(this, &WebView::loadStarted, this, [this] {
        m_isLoading = true;
    });
    connect(this, &WebView::loadFinished, this, [this] (bool ok) {
        m_isLoading = false;
        emit historyChanged(history());

        if (ok) {
            insertHistoryItem();
        }
    });

    connect(this, &WebView::iconChanged, this, [this] (const QIcon &icon) {
        Q_UNUSED(icon)

        insertHistoryItem();
    });
}

WebPage *WebView::page()
{
    return m_webPage;
}

bool WebView::isLoading() const
{
    return m_isLoading;
}

void WebView::loadHome()
{
    load(QUrl("https://google.com"));
}

void WebView::loadNewTabPage()
{
    load(QUrl("https://google.com"));
}

WebView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    switch (type) {
    case QWebEnginePage::WebBrowserTab: {
        Tab *tab = new Tab;
        emit addTabRequested(tab);
        return tab->webView();
    }
    case QWebEnginePage::WebBrowserBackgroundTab: {
        Tab *bgtab = new Tab;
        emit addTabRequested(bgtab, true);
        return bgtab->webView();
    }
    case QWebEnginePage::WebBrowserWindow: {
        Tab *wtab = Browser::instance()->createMainWindow();
        return wtab->webView();
    }
    case QWebEnginePage::WebDialog: {
        // TODO: check if dialog is disabled
        WebView *webView = new WebView;
        webView->setAttribute(Qt::WA_DeleteOnClose);
        webView->show();
        connect(webView, &WebView::titleChanged, webView, &WebView::setWindowTitle);
        return webView;
    }
    }
}

void WebView::insertHistoryItem()
{
    HistoryItem item;
    item.icon = icon();
    item.title = title();
    item.url = url().toString(QUrl::RemoveFragment);

    History::insertItem(item);

    emit historyItemInserted();
}
