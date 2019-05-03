#include "browser.h"
#include "webview.h"
#include "webpage.h"

WebView::WebView(QWidget *parent)
    : QWebEngineView (parent)
{
    m_webPage = new WebPage(Browser::instance()->profile(), this);

    setPage(m_webPage);

    load(QUrl("https://google.com"));

    connect(this, &WebView::loadStarted, this, [this] {
        m_isLoading = true;
    });
    connect(this, &WebView::loadFinished, this, [this] {
        m_isLoading = false;
        emit historyChanged(history());
    });
}

bool WebView::isLoading() const
{
    return m_isLoading;
}
