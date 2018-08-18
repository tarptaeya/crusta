#include "webview.h"
#include "webpage.h"
#include "../bootstrap/appmanager.h"
#include "../bootstrap/settings.h"
#include <QUrl>


WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
    m_webPage = new WebPage(this);
    setPage(m_webPage);
    setFocus();
}

void WebView::loadStartupUrl()
{
    appManager->settings()->beginGroup(WEBENGINE);
    const QUrl url = appManager->settings()->value(QStringLiteral("startupUrl"), QUrl("qrc:/html/speeddial/index.html")).toUrl();
    appManager->settings()->endGroup();
    load(url);
}
