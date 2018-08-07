#include "bootstrap/appmanager.h"

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    AppManager appManager;
    appManager.bootstrap();

    return application.exec();
}
