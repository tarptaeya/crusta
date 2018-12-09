#include "tools.h"
#include <QDebug>

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
