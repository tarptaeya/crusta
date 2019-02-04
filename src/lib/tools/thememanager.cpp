#include "common-defs.h"
#include "thememanager.h"

#include <QApplication>
#include <QDir>

ThemeManager::ThemeManager()
{

}

QString ThemeManager::defaultTheme()
{
    return QSL("I Prefer Native UI");
}

void ThemeManager::clearCurrentTheme()
{
    qApp->setStyleSheet(QSL(""));
}

QStringList ThemeManager::getThemeNames()
{
    QStringList themeFiles = QDir(QSL(":/themes")).entryList(QDir::Files);
    for (int i = 0; i < themeFiles.length(); i++) {
        themeFiles[i] = themeFiles[i].replace(QSL(".css"), QSL(""));
    }

    return themeFiles;
}

void ThemeManager::loadThemeFromPath(const QString &path)
{
    const QString themeData = readFile(path);
    qApp->setStyleSheet(themeData);
}
