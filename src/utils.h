#pragma once

#include <QString>
#include <QWebEngineView>

class Utils
{
public:
    static QString readFile(const QString &filePath);
    static void removeTitleBar(WId windowId);
    static void setMovableByBackground(WId windowId);
    static void setAccentColorAsWindowBackground(WId windowId);

    static QByteArray iconToByteArray(const QIcon &icon);
    static QIcon iconFromByteArray(const QByteArray &byteArray);
    static QIcon iconFromBase64(const QString &base64);

    static QWidget *webIconLabel(const QUrl &url);
};
