#include "sidebar.h"
#include "tabwidget.h"
#include "toolbar.h"
#include "window.h"

Window::Window(QWidget *parent)
    : QMainWindow (parent)
{
    m_sideBar = new SideBar;
    m_toolBar = new ToolBar;
    m_tabWidget = new TabWidget;

    QWidget *centralWidget = new QWidget;
    QHBoxLayout *centralWidgetLayout = new QHBoxLayout;

    addToolBar(m_toolBar);
    setUnifiedTitleAndToolBarOnMac(true);

    centralWidgetLayout->setSpacing(0);
    centralWidgetLayout->setContentsMargins(0, 0, 0, 0);
    centralWidgetLayout->addWidget(m_sideBar);
    centralWidget->setLayout(centralWidgetLayout);
    setCentralWidget(centralWidget);
    setContentsMargins(0, 0, 0, 0);

    centralWidgetLayout->addWidget(m_tabWidget);

    setAttribute(Qt::WA_DeleteOnClose);

    connect(m_tabWidget, &TabWidget::windowCloseRequested, this, &Window::close);
}

void Window::closeEvent(QCloseEvent *event)
{
    emit windowWillClose();

    QMainWindow::closeEvent(event);
}
