#include "common-defs.h"

#include <QDesktopServices>
#include <QFile>
#include <QUrl>

QString readFile(const QString &path)
{
    QString fileContents;
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        return fileContents;
    }
    fileContents = QString::fromUtf8(file.readAll());
    file.close();
    return fileContents;
}

QString getFileNameFromPath(const QString &path)
{
    return path.split(QSL("/")).last();
}

void openLocalFile(const QString &path)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}
