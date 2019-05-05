#include "browser.h"
#include <QApplication>
#include <QStyleFactory>
#include <QIcon>

#include <QDebug>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    // don't use native style on macos by default
    app.setStyle(QStyleFactory::create(QStringLiteral("fusion")));

    QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << QStringLiteral(":/icons/"));

    if (QIcon::fromTheme(QStringLiteral("go-home")).isNull()) {
        QIcon::setThemeName(QStringLiteral("fallback"));
    }

    Browser::instance()->run();
    int retCode = app.exec();
    Browser::instance()->deleteLater();

    return retCode;
}
