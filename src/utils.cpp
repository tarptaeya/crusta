#include "utils.h"

#ifdef __APPLE__
#include "mac/mac_utils.h"
#endif

#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QIcon>
#include <QPixmap>

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

void Utils::setMovableByBackground(WId windowId)
{
#ifdef __APPLE__
    MacUtils::setMovableByBackground(windowId);
#else
    Q_UNUSED(windowId);
#endif
}

QByteArray Utils::iconToByteArray(const QIcon &icon)
{
    QPixmap pixmap = icon.pixmap(16, 16);
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    buffer.close();

    return byteArray;
}

QIcon Utils::iconFromByteArray(const QByteArray &byteArray)
{
    QPixmap pixmap = QPixmap();
    pixmap.loadFromData(byteArray);
    return QIcon(pixmap);
}
