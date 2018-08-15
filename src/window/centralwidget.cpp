#include "centralwidget.h"
#include "../sidebar/sidebar.h"
#include "../tabview/tabwidget.h"
#include "../utils/layouts.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QSplitter(parent)
{
    TabWidget *tabWidget = new TabWidget(this);
    addWidget(tabWidget);
    m_sideBar = new SideBar(this);
    if (Layouts::sideBarPosition() == Layouts::Left) {
        insertWidget(0, m_sideBar);
    } else {
        addWidget(m_sideBar);
    }

    setHandleWidth(0);
    setStretchFactor(0, 0);
    setContentsMargins(0, 0, 0, 0);
}

CentralWidget::~CentralWidget()
{
}
