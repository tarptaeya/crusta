#include "scripts.h"

#include <QFile>
#include <QIODevice>
#include <QDebug>

Scripts::Scripts()
{
}

QString Scripts::webChannelScript()
{
    QFile file1(":qtwebchannel/qwebchannel.js");
    if (!file1.open(QIODevice::ReadOnly)) {
        qDebug() << "qwebchannel.js not found";
        return QStringLiteral("");
    }

    QFile file2(":html/webchannel.js");
    if (!file2.open(QIODevice::ReadOnly)) {
        qDebug() << "webchannel.js not found";
        return QStringLiteral("");
    }
    return QString("%1 %2").arg(QString::fromLatin1(file1.readAll()), QString::fromLatin1(file2.readAll()));
}
