#include "database.h"
#include "mainapplication.h"
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
        appManager->dataBase()->addHistory(history()->currentItem());
    });
}

WebView::~WebView()
{
    m_webPage->deleteLater();
}

bool WebView::isLoading()
{
    return m_isLoading;
}
