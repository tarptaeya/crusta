#include "database.h"
#include "browserwindow.h"
#include "historyitem.h"
#include "mainapplication.h"
#include "tab.h"
#include "tabwidget.h"
#include "webpage.h"
#include "webview.h"

#include <QWebEngineFullScreenRequest>

WebView::WebView(QWidget *parent)
    : QWebEngineView (parent)
{
    m_webPage = new WebPage(appManager->webEngineProfile());
    setPage(m_webPage);

    m_emptyWidget = new QWidget;

    int defaultZoom = appManager->settings()->value(QSL("webView/defaultZoom"), QSL("100")).toString().toInt();
    if (defaultZoom != 0) {
        setZoomFactor(defaultZoom / 100.0);
    }

    connect(this, &WebView::loadStarted, this, [this] { m_isLoading = true; });
    connect(this, &WebView::loadFinished, this, [this] {
        m_isLoading = false;
        appManager->dataBase()->addHistory(HistoryItem::fromWebEngineHistoryItem(history()->currentItem()));
    });
    connect(m_webPage, &WebPage::fullScreenRequested, this, &WebView::handleFullScreen);
}

WebView::~WebView()
{
    m_webPage->deleteLater();
    m_emptyWidget->deleteLater();
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

WebPage *WebView::page()
{
    return m_webPage;
}

void WebView::zoomIn()
{
    setZoomFactor(zoomFactor() + 0.25);
}

void WebView::zoomOut()
{
    setZoomFactor(zoomFactor() - 0.25);
}

void WebView::resetZoom()
{
    setZoomFactor(1);
}

void WebView::handleFullScreen(QWebEngineFullScreenRequest request)
{
    if (request.toggleOn()) {
        m_parent = static_cast<QWidget *>(parent());
        if (!m_parent) {
            request.reject();
            return;
        }

        m_parent->layout()->replaceWidget(this, m_emptyWidget);
        setParent(nullptr);
        showFullScreen();
    } else {
        m_parent->layout()->replaceWidget(m_emptyWidget, this);
        m_emptyWidget->setParent(nullptr);
    }

    request.accept();
}
