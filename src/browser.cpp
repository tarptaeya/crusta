#include "browser.h"
#include "crscheme.h"
#include "database.h"
#include "downloads.h"
#include "mainwindow.h"
#include "plugins.h"
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
    m_downloads = new Downloads;
    m_plugins = new Plugins(this);

    setupProfile();
}

Browser::~Browser()
{
    delete m_database;
    delete m_downloads;
}

void Browser::run()
{
    createMainWindow()->webView()->loadStart();
}

Tab *Browser::createMainWindow()
{
    MainWindow *mainWindow = new MainWindow;
    m_mainWindows.append(mainWindow);
    connect(mainWindow, &MainWindow::newMainWindowRequested, this, [this] {
        createMainWindow()->webView()->loadStart();
    });
    connect(mainWindow, &MainWindow::mainWindowWillClose, this, [this, mainWindow] { m_mainWindows.removeOne(mainWindow); });

    Tab *tab = mainWindow->createWindow();

    mainWindow->show();

    if (m_mainWindows.length() > 1) {
        QPoint topLeft = m_mainWindows.at(m_mainWindows.length() - 2)->geometry().topLeft();
        topLeft.setX(topLeft.x() + 20);
        topLeft.setY(topLeft.y() + 10);
        mainWindow->move(topLeft);
    }

    return tab;
}

Downloads *Browser::downloads()
{
    return m_downloads;
}

Plugins *Browser::plugins()
{
    return m_plugins;
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

    QString userAgent = m_profile->httpUserAgent();
    userAgent = userAgent.replace(QRegExp(QStringLiteral("QtWebEngine/[\\d\\.]+")), QStringLiteral("Crusta/2.0.0"));
    m_profile->setHttpUserAgent(userAgent);

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

    connect(m_profile, &QWebEngineProfile::downloadRequested, [this] (QWebEngineDownloadItem *item) {
        m_downloads->popup(item);
    });
}
