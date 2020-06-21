#include "tab.h"
#include "webview.h"

#include <QVBoxLayout>

Tab::Tab(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0, 0, 0, 0);
    setLayout(vbox);

    m_webView = new WebView;
    vbox->addWidget(m_webView);
}
