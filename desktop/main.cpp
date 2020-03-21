#include "browser.h"

#include <QApplication>
#include <QIcon>
#include <QStyleFactory>

int main(int argc, char **argv)
{
    QApplication::setOrganizationName(QStringLiteral("Crusta"));
    QApplication::setOrganizationDomain(QStringLiteral("desktop.crustabrowser.com"));
    QApplication::setApplicationName(QStringLiteral("Crusta"));
    QApplication::setApplicationVersion(QStringLiteral("2.0.0"));

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QIcon::setThemeSearchPaths(QIcon::themeSearchPaths() << QStringLiteral(":assets/icons"));
    QIcon::setThemeName(QStringLiteral("breeze"));
    QApplication::setStyle(QStyleFactory::create(QStringLiteral("fusion")));

    Browser browser;
    return browser.start(argc, argv);
}
