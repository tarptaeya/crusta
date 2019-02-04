#ifndef CR_QML_DOWNLOAD_ITEM_H
#define CR_QML_DOWNLOAD_ITEM_H

#include <QWebEngineDownloadItem>

class QmlWebPage;

class QmlDownloadItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mimeType READ mimeType CONSTANT)
    Q_PROPERTY(QString path READ readPath WRITE writePath)
    Q_PROPERTY(QString url READ url CONSTANT)

public:
    explicit QmlDownloadItem(QWebEngineDownloadItem *download, QObject *parent = nullptr);

    Q_INVOKABLE void accept();
    Q_INVOKABLE void cancel();
    Q_INVOKABLE QObject *webPage();

private:
    QString mimeType() const;
    QString readPath() const;
    void writePath(const QString &path);
    QString url() const;

    QWebEngineDownloadItem *m_downloadItem = nullptr;
};

#endif
