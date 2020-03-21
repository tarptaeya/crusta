#include "browser.h"
#include "browser_window.h"

#include <QApplication>
#include <QWidget>

int Browser::start(int argc, char **argv)
{
    QApplication app(argc, argv);
    create_browser_window();
    return app.exec();
}

BrowserWindow *Browser::create_browser_window() const
{
    BrowserWindow *window = new BrowserWindow;
    window->show();
    return window;
}
