#include "browser.h"
#include "browser_window.h"
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

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
    m_webpage = new WebPage(browser->web_profile(), this);
    setPage(m_webpage);

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &WebView::customContextMenuRequested, this, &WebView::show_context_menu);
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

    Tab *tab;

    switch (type) {
    case QWebEnginePage::WebBrowserTab:
        tab = window->add_new_tab();
        return tab->webview();
    case QWebEnginePage::WebBrowserBackgroundTab:
        tab = window->add_new_tab();
        return tab->webview();
    case QWebEnginePage::WebBrowserWindow:
        window = browser->create_browser_window();
        return window->tabs().at(0)->webview();
    case QWebEnginePage::WebDialog:
        tab = new Tab;
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
