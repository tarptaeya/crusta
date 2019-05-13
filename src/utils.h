#pragma once

#include <QString>
#include <QWidget>

class Utils
{
public:
    static QString readFile(const QString &filePath);
    static void removeTitleBar(WId windowId);
    static void setMovableByBackground(WId windowId);

    static QByteArray iconToByteArray(const QIcon &icon);
    static QIcon iconFromByteArray(const QByteArray &byteArray);
};
