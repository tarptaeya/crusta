#include "browser.h"
#include <QApplication>
#include <QStyleFactory>
#include <QIcon>

#include "theme.h"
#include <QDebug>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    app.setStyle(QStyleFactory::create(QStringLiteral("fusion")));

    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    QIcon::setThemeSearchPaths(QIcon::themeSearchPaths() << QStringLiteral(":/icons/"));
    QIcon::setThemeName(QStringLiteral("default"));

    Theme::setThemeName(QStringLiteral("default"));

    Browser::instance()->run();
    int retCode = app.exec();
    Browser::instance()->deleteLater();

    return retCode;
}
