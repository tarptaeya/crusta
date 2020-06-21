#include "window.h"
#include "toolbar.h"
#include "tabwidget.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    m_toolBar = new ToolBar;
    addToolBar(m_toolBar);
    setUnifiedTitleAndToolBarOnMac(true);

    m_tabWidget = new TabWidget;
    setCentralWidget(m_tabWidget);
    setContentsMargins(0, 0, 0, 0);

    setAttribute(Qt::WA_DeleteOnClose);
}
