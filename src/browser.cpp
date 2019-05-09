#include "browser.h"
#include "database.h"
#include "history.h"
#include "mainwindow.h"

Browser::Browser(bool isPrivate, QObject *parent)
    : QObject (parent)
{
    m_isPrivate = isPrivate;
    m_database = new Database(isPrivate);
    m_history = new History;

    setupProfile();
}

Browser::~Browser()
{
    delete m_database;
    delete m_history;
}

void Browser::run()
{
    createMainWindow();
}

void Browser::createMainWindow()
{
    MainWindow *mainWindow = new MainWindow;
    m_mainWindows.append(mainWindow);
    connect(mainWindow, &MainWindow::newMainWindowRequested, this, &Browser::createMainWindow);
    connect(mainWindow, &MainWindow::mainWindowWillClose, this, [this, mainWindow] { m_mainWindows.removeOne(mainWindow); });

    mainWindow->show();
}

History *Browser::history()
{
    return m_history;
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
    m_profile = m_isPrivate ? new QWebEngineProfile(this) : QWebEngineProfile::defaultProfile();
}
