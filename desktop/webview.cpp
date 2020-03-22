#include "browser.h"
#include "webview.h"
#include "webview_p.h"

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
    m_webpage = new WebPage(browser->web_profile(), this);
    load(QStringLiteral("browser:startpage"));
}

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent)
{
}
