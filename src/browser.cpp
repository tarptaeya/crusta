#include "browser.h"
#include "window.h"

#include <QFile>

void Browser::setupProfile()
{
    m_profile = new QWebEngineProfile("default", this);
}

Browser::Browser(QObject *parent)
    : QObject(parent)
{
    setupProfile();
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

QWebEngineProfile *Browser::profile() const
{
    return m_profile;
}

QString Browser::readFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) return QString();

    return file.readAll();
}
