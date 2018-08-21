#include "webview.h"
#include "webpage.h"
#include "../tabview/tabwidget.h"
#include "../bootstrap/appmanager.h"
#include "../bootstrap/settings.h"
#include <QUrl>


WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
    m_webPage = new WebPage(appManager->webEngineProfile());
    setPage(m_webPage);
    setFocus();
}

WebView::~WebView()
{
    m_webPage->deleteLater();
}

void WebView::loadStartupUrl()
{
    appManager->settings()->beginGroup(WEBENGINE);
    const QUrl url = appManager->settings()->value(QStringLiteral("startupUrl"), QUrl("qrc:/html/speeddial/index.html")).toUrl();
    appManager->settings()->endGroup();
    load(url);
}

void WebView::setVirtualTabWidget(TabWidget *tabWidget)
{
    m_tabWidget = tabWidget;
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    WebView *webview = new WebView;
    switch (type) {
    case QWebEnginePage::WebBrowserTab:
        m_tabWidget->addTab(webview);
        break;
    case QWebEnginePage::WebBrowserBackgroundTab:
        m_tabWidget->addBackgroundTab(webview);
        break;
    case QWebEnginePage::WebBrowserWindow:

        break;
    case QWebEnginePage::WebDialog:

        break;
    }
    return webview;
}
