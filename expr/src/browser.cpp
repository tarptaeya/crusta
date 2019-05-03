#include "browser.h"
#include "mainwindow.h"

Browser::Browser(QObject *parent)
    : QObject (parent)
{
    m_profile = new QWebEngineProfile(QStringLiteral("default"), this);
}

void Browser::run()
{
    createMainWindow();
}

void Browser::createMainWindow()
{
    MainWindow *mainWindow = new MainWindow;
    m_mainWindows.append(mainWindow);
    connect(mainWindow, &MainWindow::mainWindowWillClose, this, [this, mainWindow] { m_mainWindows.removeOne(mainWindow); });
    mainWindow->show();
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
