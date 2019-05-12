#include "browser.h"
#include "crscheme.h"
#include "database.h"
#include "mainwindow.h"
#include "tab.h"
#include "utils.h"
#include "webview.h"

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

Browser *Browser::instance()
{
    static Browser *b = new Browser;
    return b;
}

void Browser::setupProfile()
{
    m_profile = m_isPrivate ? new QWebEngineProfile : QWebEngineProfile::defaultProfile();

    CRScheme *handler = new CRScheme(this);
    m_profile->installUrlSchemeHandler("crusta", handler);
}
