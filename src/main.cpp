#include "bootstrap/appmanager.h"
#include <QApplication>

int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    QCoreApplication::setOrganizationName(appManager->organisationName());
    QCoreApplication::setOrganizationDomain(appManager->organisationDomain());
    QCoreApplication::setApplicationName(appManager->applicationName());
    QCoreApplication::setApplicationVersion(appManager->applicationVersionString());

    appManager->bootstrap();

    return application.exec();
}
