#include "api/ewebpage.h"
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

    QString themeName(QStringLiteral("breeze"));
    // TODO: check for other good way to check dark mode
    if (app.palette().window().color().red() < 200) {
        themeName = QStringLiteral("breeze-dark");
    }

    QIcon::setThemeName(themeName);
    Theme::setThemeName(themeName);

    EWebPage webPage;

    Browser::instance()->run();
    int retCode = app.exec();
    Browser::instance()->deleteLater();

    return retCode;
}
