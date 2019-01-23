#ifndef CR_DOWNLOAD_ITEM_WIDGET_H
#define CR_DOWNLOAD_ITEM_WIDGET_H

#include <QLabel>
#include <QProgressBar>
#include <QToolButton>
#include <QWebEngineDownloadItem>
#include <QWidget>
#include <QVBoxLayout>

class DownloadItemWidget : public QWidget
{
public:
    explicit DownloadItemWidget(QWebEngineDownloadItem *item, bool openAfterDownload, QWidget *parent = nullptr);

private:
    void updateProgress(qint64 bytesRecieved, qint64 bytesTotal);
    void postDownload(QWebEngineDownloadItem *item);

    QVBoxLayout *m_vboxLayout = nullptr;
    QHBoxLayout *m_hboxLayout = nullptr;
    QProgressBar *m_progressBar = nullptr;
    QToolButton *m_pauseButton = nullptr;
    QToolButton *m_cancelButton = nullptr;
};

#endif
