#include "browser.h"
#include "crscheme.h"
#include "database.h"
#include "mainwindow.h"
#include "tab.h"
#include "utils.h"
#include "webview.h"

#include <QDir>
#include <QWebChannel>
#include <QWebEnginePage>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

Browser::Browser(bool isPrivate, QObject *parent)
    : QObject (parent)
{
    m_isPrivate = isPrivate;
    m_database = new Database(isPrivate);

    setupProfile();
}

Browser::~Browser()
{
    delete m_database;
}

void Browser::run()
{
    createMainWindow()->webView()->loadHome();
}

Tab *Browser::createMainWindow()
{
    MainWindow *mainWindow = new MainWindow;
    m_mainWindows.append(mainWindow);
    connect(mainWindow, &MainWindow::newMainWindowRequested, this, [this] {
        createMainWindow()->webView()->loadHome();
    });
    connect(mainWindow, &MainWindow::mainWindowWillClose, this, [this, mainWindow] { m_mainWindows.removeOne(mainWindow); });

    Tab *tab = mainWindow->createWindow();

    mainWindow->show();

    return tab;
}

QWebEngineProfile *Browser::profile()
{
    return m_profile;
}

bool Browser::isPrivate() const
{
    return m_isPrivate;
}

Browser *Browser::instance(bool isPrivate)
{
    static Browser *b = new Browser(isPrivate);
    return b;
}

void Browser::setupProfile()
{
    m_profile = m_isPrivate ? new QWebEngineProfile : QWebEngineProfile::defaultProfile();

    CRScheme *handler = new CRScheme(this);
    m_profile->installUrlSchemeHandler("crusta", handler);

    QWebEngineScript webChannel;
    webChannel.setName(QStringLiteral("web-channel"));
    webChannel.setWorldId(QWebEngineScript::ApplicationWorld);
    webChannel.setSourceCode(Utils::readFile(QStringLiteral(":/qtwebchannel/qwebchannel.js")));
    webChannel.setInjectionPoint(QWebEngineScript::DocumentCreation);
    webChannel.setRunsOnSubFrames(false);
    m_profile->scripts()->insert(webChannel);

    QDir scriptsDir(QStringLiteral(":/scripts"));
    const QStringList scripts = scriptsDir.entryList();
    for (const QString &scriptPath : scripts) {
        QWebEngineScript script;
        script.setName(QStringLiteral("%1").arg(scriptPath).remove(QStringLiteral(".js")));
        script.setWorldId(QWebEngineScript::ApplicationWorld);
        script.setSourceCode(Utils::readFile(QStringLiteral(":/scripts/%1").arg(scriptPath)));
        script.setInjectionPoint(QWebEngineScript::DocumentReady);
        script.setRunsOnSubFrames(false);

        m_profile->scripts()->insert(script);
    }
}
