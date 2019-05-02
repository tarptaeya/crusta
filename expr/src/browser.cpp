#include "browser.h"
#include "window.h"

Browser::Browser(QObject *parent)
    : QObject (parent)
{
    m_profile = new QWebEngineProfile(QStringLiteral("default"), this);
}

void Browser::run()
{
    createWindow();
}

void Browser::createWindow()
{
    Window *window = new Window;
    m_windows.append(window);
    connect(window, &Window::windowWillClose, this, [this, window] { m_windows.removeOne(window); });
    window->show();
}

QWebEngineProfile *Browser::profile()
{
    return m_profile;
}

Browser *Browser::instance()
{
    static Browser *b = new Browser;
    return b;
}
