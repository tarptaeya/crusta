#include "common-defs.h"
#include "mainapplication.h"
#include "thememanager.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QCommandLineParser parser;
    QCommandLineOption privateModeOption(QStringList() << QSL("p") << QSL("private"));
    parser.addOption(privateModeOption);

    parser.process(app);

    MainApplication::instance(parser.isSet(privateModeOption))->createWindow();
    const QString defaultTheme = appManager->settings()->value(QSL("theme/defaultTheme"), ThemeManager::defaultTheme()).toString();
    if (defaultTheme == QSL("I Prefer Native UI")) {
#ifdef Q_OS_MAC
        const QString defaultTheme = readFile(QSL(":/styles/styles.mac.css"));
        app.setStyleSheet(defaultTheme);
#endif
    } else if (defaultTheme == QSL("Use Custom CSS")) {
        const QString customTheme = appManager->settings()->value(QSL("theme/customTheme")).toString();
        app.setStyleSheet(customTheme);
    } else {
        ThemeManager::loadThemeFromPath(QSL(":/themes/%1.css").arg(defaultTheme));
    }

    int retCode = app.exec();

    delete MainApplication::instance();

    return retCode;
}
