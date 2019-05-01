#include "browser.h"
#include "window.h"

Browser::Browser(QObject *parent)
    : QObject (parent)
{
}

void Browser::run()
{
    createWindow();
}

void Browser::createWindow()
{
    Window *window = new Window;
    m_windows.append(window);
    window->show();
}
