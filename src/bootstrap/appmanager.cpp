#include "appmanager.h"
#include "../window/window.h"
#include "settings.h"
#include "database.h"
#include "../pages/scripts.h"

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

    setUpWebEngineProfile();

    m_database = new Database(this);
    m_database->createDatabases();
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

QWebEngineProfile *AppManager::webEngineProfile() const
{
    return m_webEngineProfile;
}

Database *AppManager::database() const
{
    return m_database;
}

void AppManager::setUpWebEngineProfile()
{
    m_webEngineProfile = new QWebEngineProfile(this);

    QWebEngineScript webChannelScript;
    webChannelScript.setInjectionPoint(QWebEngineScript::DocumentCreation);
    webChannelScript.setName("webChannelScript");
    webChannelScript.setRunsOnSubFrames(true);
    webChannelScript.setSourceCode(Scripts::webChannelScript());
    webChannelScript.setWorldId(QWebEngineScript::ApplicationWorld);
    m_webEngineProfile->scripts()->insert(webChannelScript);

    QWebEngineScript speeddialScript;
    speeddialScript.setInjectionPoint(QWebEngineScript::DocumentReady);
    speeddialScript.setName("speeddialScript");
    speeddialScript.setRunsOnSubFrames(true);
    speeddialScript.setSourceCode(Scripts::speeddialScript());
    speeddialScript.setWorldId(QWebEngineScript::ApplicationWorld);
    m_webEngineProfile->scripts()->insert(speeddialScript);
}
