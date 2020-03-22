#include "browser.h"
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

    load(QStringLiteral("browser:startpage"));

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &WebView::customContextMenuRequested, this, &WebView::show_context_menu);
}

void WebView::home()
{
    // TODO: load homepage
}

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent)
{
}
