#include "browser.h"
#include "browser_window.h"
#include "history.h"
#include "tab.h"
#include "webview.h"
#include "webview_p.h"

#include <QMenu>
#include <QStyleFactory>

void WebView::show_context_menu(const QPoint &pos)
{
    QMenu *menu = m_webpage->createStandardContextMenu();
#ifdef Q_OS_MACOS
    menu->setStyle(QStyleFactory::create(QStringLiteral("macintosh")));
#endif
    menu->exec(mapToGlobal(pos));
}

void WebView::save_history_entry()
{
    HistoryEntry entry;
    entry.title = title();
    entry.address = url().toString();
    entry.icon = icon();
    entry.last_visited = QDateTime::currentDateTime();
    browser->history_model()->add_entry(entry);
}

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
    m_webpage = new WebPage(browser->web_profile(), this);
    setPage(m_webpage);

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &WebView::customContextMenuRequested, this, &WebView::show_context_menu);
    connect(this, &WebView::loadFinished, this, &WebView::save_history_entry);
    connect(this, &WebView::titleChanged, this, &WebView::save_history_entry);
    connect(this, &WebView::iconChanged, this, &WebView::save_history_entry);
}

void WebView::home()
{
    // TODO: load homepage
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    QWidget *root = parentWidget();
    while (root->parentWidget()) {
        root = root->parentWidget();
    }

    BrowserWindow *window = dynamic_cast<BrowserWindow *>(root);
    if (!window) return nullptr;

    WebTab *tab;

    switch (type) {
    case QWebEnginePage::WebBrowserTab:
        tab = dynamic_cast<WebTab *>(window->add_new_tab());
        if (!tab) return nullptr;
        return tab->webview();
    case QWebEnginePage::WebBrowserBackgroundTab:
        tab = dynamic_cast<WebTab *>(window->add_new_tab());
        if (!tab) return nullptr;
        return tab->webview();
    case QWebEnginePage::WebBrowserWindow:
        window = browser->create_browser_window();
        tab = dynamic_cast<WebTab *>(window->tabs().at(0));
        if (!tab) return nullptr;
        return tab->webview();
    case QWebEnginePage::WebDialog:
        tab = new WebTab;
        tab->setAttribute(Qt::WA_DeleteOnClose);
        tab->toolbar()->setEnabled(false);
        tab->resize(600, 400);
        tab->show();
        return tab->webview();
    }
}

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent)
{
}
