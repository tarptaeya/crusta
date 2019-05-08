#ifndef CRUSTA_UTILS_H
#define CRUSTA_UTILS_H

#include <QString>
#include <QWidget>

class Utils
{
public:
    static QString readFile(const QString &filePath);
    static void removeTitleBar(WId windowId);
};

#endif
