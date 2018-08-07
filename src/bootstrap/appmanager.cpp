#include "appmanager.h"
#include "../window/window.h"

AppManager::AppManager(QObject *parent)
    : QObject(parent)
{
}

AppManager::~AppManager()
{
    for (Window *window : m_windowList) {
        window->close();
    }
}

void AppManager::bootstrap()
{
    Window *window = new Window();
    window->show();

    m_windowList.append(window);
}
