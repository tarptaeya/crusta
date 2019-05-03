#include "browser.h"
#include "webview.h"
#include "webpage.h"

WebView::WebView(QWidget *parent)
    : QWebEngineView (parent)
{
    m_webPage = new WebPage(Browser::instance()->profile(), this);

    setPage(m_webPage);

    load(QUrl("https://google.com"));
}
