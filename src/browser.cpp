#include "browser.h"

Browser::Browser(QObject *parent)
    : QObject(parent)
{
}

Browser *Browser::instance()
{
    static Browser *browser = new Browser;
    return browser;
}

void Browser::run()
{
}
