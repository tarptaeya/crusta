#ifndef CR_DOWNLOADS_MANAGER_H
#define CR_DOWNLOADSS_MANAGER_H

#include <QListWidget>
#include <QWebEngineDownloadItem>
#include <QWidget>

class DownloadsManager : public QWidget
{
public:
    explicit DownloadsManager(QWidget *parent = nullptr);

    void downloadRequested(QWebEngineDownloadItem *item);

private:
    void askBeforeDownloading(QWebEngineDownloadItem *item);
    void downloadItem(QWebEngineDownloadItem *item, bool openAfterDownload = false);

    QListWidget *m_listWidget = nullptr;
};

#endif
