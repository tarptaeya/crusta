#include "browser.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Browser::instance()->run();
    int retCode = app.exec();
    Browser::instance()->deleteLater();
    return retCode;
}
