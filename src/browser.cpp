#include "browser.h"
#include "window.h"

Browser::Browser(QObject *parent)
    : QObject(parent)
{
}

Browser *Browser::instance()
{
    static Browser *browser = new Browser;
    return browser;
}

void Browser::run()
{
    createWindow();
}

Window *Browser::createWindow()
{
    Window *window = new Window;
    window->show();

    m_windows.append(window);
    connect(window, &Window::destroyed, [=]() {m_windows.removeOne(window);});

    return window;
}
