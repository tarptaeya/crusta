#pragma once

#include <QString>
#include <QWidget>

class Utils
{
public:
    static QString readFile(const QString &filePath);
    static void removeTitleBar(WId windowId);
};
