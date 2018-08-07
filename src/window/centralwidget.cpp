#include "centralwidget.h"
#include "../sidebar/sidebar.h"
#include "../tabview/tabview.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QSplitter(parent)
{
    m_sideBar = new SideBar(this);
    TabView *tabView = new TabView(this);
    m_tabViewList.append(tabView);
    addWidget(m_sideBar);
    addWidget(tabView);

    setStretchFactor(0, 0);
}

CentralWidget::~CentralWidget()
{
}
