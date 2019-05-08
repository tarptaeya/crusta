#include "utils.h"

#ifdef __APPLE__
#include "mac/mac_utils.h"
#endif

#include <QFile>
#include <QApplication>

QString Utils::readFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly)) {
        return QString();
    }

    return QString::fromUtf8(file.readAll());
}

void Utils::removeTitleBar(WId windowId)
{
#ifdef __APPLE__
    MacUtils::removeTitleBar(windowId);
#else
    Q_UNUSED(windowId);
#endif
}
