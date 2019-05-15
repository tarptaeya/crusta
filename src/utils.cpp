#include "utils.h"

#ifdef __APPLE__
#include "mac/mac_utils.h"
#endif

#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QIcon>
#include <QPixmap>
#include <QWebEngineProfile>

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

QWidget *Utils::webIconLabel(const QUrl &url)
{
    QWebEngineView *view = new QWebEngineView;
    QWebEngineProfile *profile = new QWebEngineProfile;
    QWebEnginePage *page = new QWebEnginePage(profile);

    view->setPage(page);

    view->setHtml(QStringLiteral("<!DOCTYPE html>"
                                 "<html>"
                                 "<head>"
                                 "<style>"
                                 "html, body {"
                                 "width: 100%;"
                                 "height: 100%;"
                                 "margin: 0;"
                                 "}"
                                 "body {"
                                 "background-image: url(%1);"
                                 "background-size: cover;"
                                 "background-repeat: norepeat;"
                                 "background-color: transparent;"
                                 "}"
                                 "</style>"
                                 "</head>"
                                 "<body>"
                                 "</body>"
                                 "</html>").arg(url.toString()));

    view->setFixedSize(24, 24);
    QObject::connect(view, &QWebEngineView::destroyed, [page, profile] {
        page->deleteLater();
        profile->deleteLater();
    });

    return view;
}
