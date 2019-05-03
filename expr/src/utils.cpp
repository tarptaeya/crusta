#include "utils.h"

#include <QFile>

QString Utils::readFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly)) {
        return QString();
    }

    return QString::fromUtf8(file.readAll());
}
