#include "tab.h"
#include "webview.h"

Tab::Tab(QWidget *parent)
    : QWidget (parent)
{
    m_webView = new WebView;
    m_layout = new QVBoxLayout;

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->addWidget(m_webView);
    setLayout(m_layout);
}

WebView *Tab::webView() const
{
    return m_webView;
}
