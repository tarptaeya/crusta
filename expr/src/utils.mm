#include "utils.h"

#include <QFile>
#include <QApplication>

#ifdef __APPLE__
#include <Cocoa/Cocoa.h>
#endif

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
    NSView *nativeView = reinterpret_cast<NSView *>(windowId);
    NSWindow *nativeWindow = [nativeView window];
    [nativeWindow setTitlebarAppearsTransparent:YES];
#endif
}
