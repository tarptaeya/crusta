#include "browser.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    // don't use native style on macos by default
    app.setStyle(QStyleFactory::create(QStringLiteral("fusion")));

    Browser::instance()->run();
    int retCode = app.exec();
    Browser::instance()->deleteLater();

    return retCode;
}
