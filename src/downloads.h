#pragma once

#include <QListWidget>
#include <QWebEngineDownloadItem>
#include <QWidget>

class Downloads : public QWidget
{
    Q_OBJECT
public:
    explicit Downloads(QWidget *parent = nullptr);
    void popup(QWebEngineDownloadItem *item);

Q_SIGNALS:
    void downloadAdded(QWebEngineDownloadItem *item);

private:
    QListWidget *m_listWidget = nullptr;

    void addDownload(QWebEngineDownloadItem *item);
};
