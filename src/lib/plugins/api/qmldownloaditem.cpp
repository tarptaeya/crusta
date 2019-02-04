#include "common-defs.h"
#include "qmldownloaditem.h"
#include "qmlwebpage.h"

#include <QQmlEngine>

QmlDownloadItem::QmlDownloadItem(QWebEngineDownloadItem *download, QObject *parent)
    : QObject (parent)
    , m_downloadItem(download)
{
    qmlEngine(this)->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

void QmlDownloadItem::accept()
{
    if (!m_downloadItem) {
        return;
    }

    m_downloadItem->accept();
}

void QmlDownloadItem::cancel()
{
    if (!m_downloadItem) {
        return;
    }

    m_downloadItem->cancel();
}

QObject *QmlDownloadItem::webPage()
{
    if (!m_downloadItem) {
        return nullptr;
    }

    return new QmlWebPage(m_downloadItem->page());
}

QString QmlDownloadItem::mimeType() const
{
    if (!m_downloadItem) {
        return QSL("");
    }

    return m_downloadItem->mimeType();
}

QString QmlDownloadItem::readPath() const
{
    if (!m_downloadItem) {
        return QSL("");
    }
    return m_downloadItem->path();
}

void QmlDownloadItem::writePath(const QString &path)
{
    if (!m_downloadItem) {
        return;
    }

    m_downloadItem->setPath(path);
}

QString QmlDownloadItem::url() const
{
    if (!m_downloadItem) {
        return QSL("");
    }

    return QString::fromUtf8(m_downloadItem->url().toEncoded());
}
