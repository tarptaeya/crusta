#include "about.h"
#include "common-defs.h"
#include "browserwindow.h"
#include "database.h"
#include "downloadsmanager.h"
#include "mainapplication.h"
#include "manager.h"
#include "plugins.h"
#include "scheme.h"
#include "scripts.h"
#include "standardpaths.h"
#include "tab.h"
#include "webdialogwindow.h"

#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

MainApplication::MainApplication(QObject *parent)
    : QObject (parent)
{
    loadPlugins();
}

MainApplication::~MainApplication()
{
    m_dataBase->deleteLater();
    m_manager->deleteLater();
    m_settings->deleteLater();

    m_webEngineProfile->deleteLater();
    m_plugins->deleteLater();
}

MainApplication *MainApplication::instance()
{
    static MainApplication *mainApplication = new MainApplication;
    return static_cast<MainApplication *>(mainApplication);
}

BrowserWindow *MainApplication::createWindow()
{
    const QString startPageUrl = settings()->value(QSL("webView/startPageUrl"), QSL("crusta://speeddial")).toString();

    Tab *tab = new Tab(startPageUrl);
    return createWindow(tab);
}

BrowserWindow *MainApplication::createWindow(Tab *tab)
{
    BrowserWindow *window = new BrowserWindow(tab);
    window->show();

    m_windows.append(window);

    return window;
}

void MainApplication::removeWindow()
{

}

QList<BrowserWindow *> MainApplication::windows()
{
    return m_windows;
}

BrowserWindow *MainApplication::currentWindow()
{
    for (int i = 0; i < m_windows.length(); i++) {
        if (m_windows.at(i)->isActiveWindow()) {
            return m_windows.at(i);
        }
    }

    return m_windows.last();
}

void MainApplication::createDialogWindow(WebView *webView)
{
    WebDialogWindow *webDialogWindow = new WebDialogWindow(webView);
    webDialogWindow->show();
}

DataBase *MainApplication::dataBase()
{
    if (!m_dataBase) {
        m_dataBase = new DataBase;
    }
    return m_dataBase;
}

Manager *MainApplication::manager()
{
    if (!m_manager) {
        m_manager = new Manager;
    }
    return m_manager;
}

QSettings *MainApplication::settings()
{
    if (!m_settings) {
        m_settings = new QSettings(StandardPaths::settingsPath(), QSettings::IniFormat);
    }
    return m_settings;
}

void MainApplication::initWebEngineProfile()
{
    m_webEngineProfile = new QWebEngineProfile(this);

    QWebEngineScript webChannelScript;
    webChannelScript.setName(QSL("webChannelScript"));
    webChannelScript.setInjectionPoint(QWebEngineScript::DocumentCreation);
    webChannelScript.setWorldId(QWebEngineScript::ApplicationWorld);
    webChannelScript.setRunsOnSubFrames(false);
    webChannelScript.setSourceCode(Scripts::webChannelScript());
    m_webEngineProfile->scripts()->insert(webChannelScript);

    for (const Script &script : Scripts::loadInternalScripts()) {
        QWebEngineScript webEngineScript;
        webEngineScript.setName(script.name);
        webEngineScript.setInjectionPoint(QWebEngineScript::DocumentReady);
        webEngineScript.setWorldId(QWebEngineScript::ApplicationWorld);
        webEngineScript.setRunsOnSubFrames(false);
        webEngineScript.setSourceCode(script.source);
        m_webEngineProfile->scripts()->insert(webEngineScript);
    }

    Scheme *scheme = new Scheme(this);
    m_webEngineProfile->installUrlSchemeHandler("crusta", scheme);

    connect(m_webEngineProfile, &QWebEngineProfile::downloadRequested, manager()->downloadsManager(), &DownloadsManager::downloadRequested);
}

QWebEngineProfile *MainApplication::webEngineProfile()
{
    if (!m_webEngineProfile) {
        initWebEngineProfile();
    }

    return m_webEngineProfile;
}

void MainApplication::loadPlugins()
{
    m_plugins = new Plugins(this);
    m_plugins->loadPlugins();
}

Plugins *MainApplication::plugins()
{
    return m_plugins;
}

void MainApplication::aboutCrusta()
{
    About about;
    about.exec();
}
