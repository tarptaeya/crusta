#include "window.h"
#include "centralwidget.h"
#include "../utils/strings.h"

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setObjectName(Strings::windowName());

    m_isPrivate = false;
    m_centralWidget = new CentralWidget(this);
    setCentralWidget(m_centralWidget);
}

Window::~Window()
{
}
