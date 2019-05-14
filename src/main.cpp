#include "api/ewebpage.h"
#include "browser.h"
#include "theme.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QStyleFactory>
#include <QIcon>
#include <QWebEngineUrlScheme>

#include <iostream>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    app.setStyle(QStyleFactory::create(QStringLiteral("fusion")));

    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    app.setOrganizationName(QStringLiteral("Crusta"));
    app.setOrganizationDomain(QStringLiteral("crustabrowser.com"));
    app.setApplicationName(QStringLiteral("Crusta"));
    app.setApplicationVersion(QStringLiteral("2.0.0"));

    QCommandLineParser parser;
    QCommandLineOption privateOption(QStringLiteral("private"), QCoreApplication::translate("main", "Open browser in private mode"));
    parser.addOption(privateOption);

    parser.process(app);

    QIcon::setThemeSearchPaths(QIcon::themeSearchPaths() << QStringLiteral(":/icons/"));

    QString themeName(QStringLiteral("breeze"));
    // TODO: check for other good way to check dark mode
    if (app.palette().window().color().red() < 200) {
        themeName = QStringLiteral("breeze-dark");
    }

    QIcon::setThemeName(themeName);
    Theme::setThemeName(themeName);

    QWebEngineUrlScheme scheme("crusta");
    scheme.setFlags(QWebEngineUrlScheme::SecureScheme);
    QWebEngineUrlScheme::registerScheme(scheme);

    EWebPage webPage;

    bool isPrivate = parser.isSet(privateOption);

    Browser::instance(isPrivate)->run();
    int retCode = app.exec();
    Browser::instance()->deleteLater();

    return retCode;
}
