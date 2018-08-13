#include "appmanager.h"
#include "../window/window.h"
#include "settings.h"

AppManager::AppManager(QObject *parent)
    : QObject(parent)
{
    m_application.organisationName = QStringLiteral("crusta");
    m_application.organisationDomain = QStringLiteral("crusta.io");
    m_application.name = QStringLiteral("Crusta - Fast, Secure & Modern Web-browser");
    m_application.majorVersion = 2;
    m_application.minorVersion = 0;
    m_application.patchVersion = 0;
    // set developmentBuild to false when releasing
    m_application.mode = Development;
}

AppManager::~AppManager()
{
    for (Window *window : m_windowList) {
        window->close();
    }
}

AppManager *AppManager::instance()
{
    static AppManager *applicationManager = new AppManager();
    return applicationManager;
}

QString AppManager::organisationName()
{
    return m_application.organisationName;
}

QString AppManager::organisationDomain()
{
    return m_application.organisationDomain;
}

QString AppManager::applicationName()
{
    return m_application.name;
}

int AppManager::majorVersion()
{
    return m_application.majorVersion;
}

int AppManager::minorVersion()
{
    return m_application.minorVersion;
}

int AppManager::patchVersion()
{
    return m_application.patchVersion;
}

QString AppManager::applicationVersionString()
{
    QString versionString = m_application.majorVersion + QStringLiteral(".") + m_application.minorVersion + QStringLiteral(".") + m_application.patchVersion;
    if (m_application.mode == Development) {
        versionString += QStringLiteral("+dev");
    } else if (m_application.mode == Alpha) {
        versionString += QStringLiteral("+alpha");
    } else if (m_application.mode == Beta) {
        versionString += QStringLiteral("+beta");
    }
    return versionString;
}


void AppManager::bootstrap()
{
    Window *window = new Window();
    window->show();

    m_windowList.append(window);
}

Settings *AppManager::settings()
{
    if (!m_settings) {
        m_settings = new Settings(this);
    }
    return m_settings;
}
