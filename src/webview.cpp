#include "browser.h"
#include "history.h"
#include "tab.h"
#include "webview.h"
#include "webpage.h"

#include <QMenu>
#include <QSettings>
#include <QWebEngineSettings>

WebView::WebView(QWidget *parent)
    : QWebEngineView (parent)
{
    m_webPage = new WebPage(Browser::instance()->profile(), this);
    setPage(m_webPage);

    loadSettings();

    setContextMenuPolicy(Qt::CustomContextMenu);

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

    connect(this, &WebView::customContextMenuRequested, this, &WebView::showContextMenu);
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
    QSettings settings;
    QString url = settings.value(QStringLiteral("browsing/homepage"), QStringLiteral("crusta:startpage")).toString();
    load(url);
}

void WebView::loadNewTabPage()
{
    QSettings settings;
    QString url = settings.value(QStringLiteral("browsing/newtabpage"), QStringLiteral("crusta:startpage")).toString();
    load(url);
}

void WebView::loadStart()
{
    QSettings settings;
    QString url = settings.value(QStringLiteral("browsing/startpage"), QStringLiteral("crusta:startpage")).toString();
    load(url);
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

void WebView::zoomIn()
{
    setZoomFactor(zoomFactor() + 0.25);
}

void WebView::zoomOut()
{
    setZoomFactor(zoomFactor() - 0.25);
}

void WebView::zoomReset()
{
    setZoomFactor(1.00);
}

void WebView::loadSettings()
{
    QSettings qset;

    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
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

void WebView::showContextMenu(const QPoint &pos)
{
    QMenu *menu = m_webPage->createStandardContextMenu();
    menu->exec(mapToGlobal(pos));
}
