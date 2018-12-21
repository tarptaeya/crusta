#include "tools.h"
#include "appmanager.h"
#include "settings.h"
#include <QDebug>
#include <QApplication>

QString readFile(const QString &fileName)
{
    QString fileContents;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        return fileContents;
    }
    fileContents = QString::fromUtf8(file.readAll());
    file.close();
    return fileContents;
}

QByteArray convertIconToByteArray(const QIcon &icon)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    icon.pixmap(16, 16).save(&buffer, "PNG");
    return byteArray;
}

void applyTheme()
{
    QString themeData = readFile(":/styles/stylesheet.css");

    appManager->settings()->beginGroup(THEME);
    themeData = themeData.replace("%TOOLBAR-COLOR%", appManager->settings()->value("TOOLBAR-COLOR", "#FFFFFF").toString());
    themeData = themeData.replace("%TABBAR-COLOR%", appManager->settings()->value("TABBAR-COLOR", "#7AAF5D").toString());
    themeData = themeData.replace("%ACTIVE-COLOR%", appManager->settings()->value("ACTIVE-COLOR", "#CA8D42").toString());
    themeData = themeData.replace("%ACTIVE-TEXT-COLOR%", appManager->settings()->value("ACTIVE-TEXT-COLOR", "#000000").toString());
    themeData = themeData.replace("%PASSIVE-TEXT-COLOR%", appManager->settings()->value("PASSIVE-TEXT-COLOR", "#FFFFFF").toString());
    appManager->settings()->endGroup();

    qApp->setStyleSheet(themeData);
}
