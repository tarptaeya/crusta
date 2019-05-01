#include "browser.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Browser browser;
    browser.run();

    return app.exec();
}
