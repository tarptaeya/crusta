#include "tab.h"
#include "../webview/webview.h"
#include <QVBoxLayout>

Tab::Tab(QWidget *parent)
    : QWidget(parent)
{
    m_title = tr("New Tab");
    m_webView = new WebView(this);

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    setLayout(vBoxLayout);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);
    vBoxLayout->addWidget(m_webView);
}

QString Tab::title() const
{
    return m_title;
}
