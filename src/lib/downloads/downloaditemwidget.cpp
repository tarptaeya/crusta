#include "common-defs.h"
#include "downloaditemwidget.h"

#include <QPushButton>

DownloadItemWidget::DownloadItemWidget(QWebEngineDownloadItem *item, bool openAfterDownload, QWidget *parent)
    : QWidget (parent)
{
    const QString fileName = getFileNameFromPath(item->path());

    m_vboxLayout = new QVBoxLayout;
    m_vboxLayout->setContentsMargins(2, 2, 2, 2);

    QLabel *infoLabel = new QLabel;
    infoLabel->setText(fileName);

    QHBoxLayout *hboxLayout1 = new QHBoxLayout;
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    hboxLayout1->addWidget(infoLabel, 1);

    m_progressBar = new QProgressBar;
    m_progressBar->setMinimum(0);
    m_pauseButton = new QToolButton;
    m_pauseButton->setToolTip(QSL("Pause"));
    m_pauseButton->setIcon(QIcon::fromTheme(QSL("media-playback-pause")));
    m_cancelButton = new QToolButton;
    m_cancelButton->setToolTip(QSL("Cancel Download"));
    m_cancelButton->setIcon(QIcon::fromTheme(QSL("process-stop")));

    m_hboxLayout = new QHBoxLayout;
    m_hboxLayout->setContentsMargins(0, 0, 0, 0);
    m_hboxLayout->addWidget(m_progressBar, 1);
    m_hboxLayout->addWidget(m_pauseButton);
    m_hboxLayout->addWidget(m_cancelButton);

    m_vboxLayout->addLayout(hboxLayout1);
    m_vboxLayout->addLayout(m_hboxLayout);

    setLayout(m_vboxLayout);

    connect(m_cancelButton, &QToolButton::clicked, item, &QWebEngineDownloadItem::cancel);
    connect(m_pauseButton, &QToolButton::clicked, item, [this, item] {
        if (item->isPaused()) {
            item->resume();
            m_pauseButton->setIcon(QIcon::fromTheme(QSL("media-playback-pause")));
        } else {
            item->pause();
            m_pauseButton->setIcon(QIcon::fromTheme(QSL("media-playback-start")));
        }
    });

    connect(item, &QWebEngineDownloadItem::downloadProgress, this, &DownloadItemWidget::updateProgress);
    connect(item, &QWebEngineDownloadItem::finished, this, [this, item, openAfterDownload] {
        if (openAfterDownload) {
            openLocalFile(item->path());
        }

        postDownload(item);
    });
}

void DownloadItemWidget::updateProgress(qint64 bytesRecieved, qint64 bytesTotal)
{
    if (bytesTotal == 0) {
        m_progressBar->setMaximum(0);
        m_progressBar->setValue(0);
        return;
    }

    int progress = static_cast<int>((bytesRecieved * 100) / bytesTotal);
    m_progressBar->setMaximum(100);
    m_progressBar->setValue(progress);
}

void DownloadItemWidget::postDownload(QWebEngineDownloadItem *item)
{
    if (!item) {
        return;
    }

    m_hboxLayout->removeWidget(m_progressBar);
    m_hboxLayout->removeWidget(m_pauseButton);
    m_hboxLayout->removeWidget(m_cancelButton);
    m_progressBar->deleteLater();
    m_pauseButton->deleteLater();
    m_cancelButton->deleteLater();

    QLabel *postDownloadInfo = new QLabel;

    if (item->state() != QWebEngineDownloadItem::DownloadCompleted) {
        m_hboxLayout->addWidget(postDownloadInfo);
        return;
    }

    postDownloadInfo->setText(QSL("Download Finished."));
    QPushButton *openButton = new QPushButton(QSL("Open"));
    m_hboxLayout->addWidget(postDownloadInfo, 1);
    m_hboxLayout->addWidget(openButton);

    connect(openButton, &QPushButton::clicked, this, [item] { openLocalFile(item->path()); });
}
