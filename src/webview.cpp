#include "webview.h"
#include "webpage.h"
#include "browser.h"

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
    m_webPage = new WebPage(crusta->profile(), this);
    setPage(m_webPage);
}
