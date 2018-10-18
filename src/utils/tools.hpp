#pragma once

#include <QString>
#include <QFile>

#define QSL QStringLiteral

namespace CrTools {

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

}
