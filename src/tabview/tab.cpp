#include "tab.h"
#include "tabwidget.h"
#include "../webview/webview.h"
#include <QVBoxLayout>

Tab::Tab(QWidget *parent, WebView *webview)
    : QWidget(parent)
{
    m_webView = webview;
    if (!m_webView) {
        m_webView = new WebView(this);
    }

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    setLayout(vBoxLayout);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);
    vBoxLayout->addWidget(m_webView);
}

WebView *Tab::webview() const
{
    return m_webView;
}

TabListItem *Tab::tabListItem() const
{
    return m_tabListItem;
}

void Tab::setTabListItem(TabListItem *tabListItem)
{
    m_tabListItem = tabListItem;
}

void Tab::closeTab()
{
    deleteLater();
}

void Tab::setVirtualTabWidget(TabWidget *tabWidget)
{
    m_tabWidget = tabWidget;
    m_webView->setVirtualTabWidget(tabWidget);
}
