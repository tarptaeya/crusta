#include "src/browser.h"
#include "ewindows.h"

#include <QDebug>

EWindows::EWindows(QObject *parent)
    : QObject (parent)
{
}

void EWindows::create()
{
    Browser::instance()->createMainWindow();
}
