#include "tab.h"
#include "webview.h"

#include <QVBoxLayout>

Tab::Tab(QWidget *parent)
    : QWidget(parent)
{
    m_webview = new WebView;

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);
    setLayout(vbox);

    vbox->addWidget(m_webview);
}
