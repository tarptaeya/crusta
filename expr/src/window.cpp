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
    connect(m_toolBar, &ToolBar::backRequested, m_tabWidget, &TabWidget::back);
    connect(m_toolBar, &ToolBar::forwardRequested, m_tabWidget, &TabWidget::forward);
    connect(m_tabWidget, &TabWidget::urlChanged, m_toolBar, &ToolBar::setUrl);
    connect(m_tabWidget, &TabWidget::historyChanged, m_toolBar, &ToolBar::setHistory);
    connect(m_toolBar, &ToolBar::navigationToItemRequest, m_tabWidget, &TabWidget::navigateToItem);
    connect(m_tabWidget, &TabWidget::loadStarted, m_toolBar, &ToolBar::loadStarted);
    connect(m_tabWidget, &TabWidget::loadFinished, m_toolBar, &ToolBar::loadFinished);
    connect(m_toolBar, &ToolBar::loadingStateChangeRequest, m_tabWidget, &TabWidget::changeLoadingState);
}

void Window::closeEvent(QCloseEvent *event)
{
    emit windowWillClose();

    QMainWindow::closeEvent(event);
}
