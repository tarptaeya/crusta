#include "window.h"
#include "tabwidget.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    m_tabWidget = new TabWidget;
    setCentralWidget(m_tabWidget);
    setContentsMargins(0, 0, 0, 0);

    setAttribute(Qt::WA_DeleteOnClose);
}
