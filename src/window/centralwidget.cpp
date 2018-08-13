#include "centralwidget.h"
#include "../sidebar/sidebar.h"
#include "../tabview/tabview.h"
#include "../utils/layouts.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QSplitter(parent)
{
    TabView *tabView = new TabView(this);
    addWidget(tabView);
    m_sideBar = new SideBar(this);
    if (Layouts::sideBarPosition() == Layouts::Left) {
        insertWidget(0, m_sideBar);
    } else {
        addWidget(m_sideBar);
    }

    setStretchFactor(0, 0);
}

CentralWidget::~CentralWidget()
{
}
