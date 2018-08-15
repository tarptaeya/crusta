#include "tabwidget.h"
#include "tabbar.h"
#include "stackedwidget.h"
#include <QVBoxLayout>

TabWidget::TabWidget(QWidget *parent)
    : QWidget(parent)
{
    m_tabBar = new TabBar(this);
    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    setLayout(vBoxLayout);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);
    vBoxLayout->addWidget(m_tabBar);

    m_stakedWidget = new StackedWidget(this);
    vBoxLayout->addWidget(m_stakedWidget);
}

TabWidget::~TabWidget()
{
}
