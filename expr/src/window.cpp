#include "sidebar.h"
#include "tabwidget.h"
#include "toolbar.h"
#include "window.h"

Window::Window(QWidget *parent)
    : QMainWindow (parent)
{
    m_sideBar = new SideBar;
    m_toolBar = new ToolBar;

    m_centralWidget = new QWidget;
    m_centralWidgetLayout = new QHBoxLayout;

    addToolBar(m_toolBar);
    setUnifiedTitleAndToolBarOnMac(true);

    m_centralWidgetLayout->setSpacing(0);
    m_centralWidgetLayout->setContentsMargins(0, 0, 0, 0);
    m_centralWidgetLayout->addWidget(m_sideBar);
    m_centralWidget->setLayout(m_centralWidgetLayout);
    setCentralWidget(m_centralWidget);
    setContentsMargins(0, 0, 0, 0);

    createTabWidget();
}

void Window::createTabWidget()
{
    TabWidget *tabWidget = new TabWidget;
    m_centralWidgetLayout->addWidget(tabWidget);

    m_tabWidgets.append(tabWidget);

    connect(tabWidget, &TabWidget::windowCloseRequested, this, [this] {
        // TODO handle case for split tabs
        close();
    });
}
