#include "downloads.h"
#include "utils.h"
#include "webpage.h"

#include <QDialog>
#include <QDir>
#include <QFileInfo>
#include <QGridLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QToolButton>

Downloads::Downloads(QWidget *parent)
    : QWidget (parent)
{
    m_listWidget = new QListWidget(this);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    setLayout(vboxLayout);

    vboxLayout->addWidget(m_listWidget);
    vboxLayout->setContentsMargins(1, 1, 1, 1);

    setWindowFlag(Qt::Popup);
    Utils::setMovableByBackground(winId());

    connect(this, &Downloads::downloadAdded, &Downloads::addDownload);
}

void Downloads::popup(QWebEngineDownloadItem *item)
{
    QFileInfo info(item->path());

    QSettings settings;
    if (!settings.value(QStringLiteral("downloads/ask"), false).toBool()) {
        const QString path = settings.value(QStringLiteral("downloads/path"), info.dir().absolutePath()).toString();
        item->setPath(QDir(path).filePath(info.fileName()));
        item->accept();
        addDownload(item);
        return ;
    }

    QDialog dialog;
    dialog.setWindowFlag(Qt::FramelessWindowHint);

    QLabel *heading = new QLabel(QStringLiteral("<b>Download Requested</b>"));
    QLabel *fileLabel = new QLabel(QStringLiteral("File"));
    QLabel *file = new QLabel(info.fileName());

    QLabel *mimeLabel = new QLabel(QStringLiteral("Mimetype"));
    QLabel *mime = new QLabel(item->mimeType());

    QPushButton *cancel = new QPushButton(QStringLiteral("Cancel"));
    QPushButton *accept = new QPushButton(QStringLiteral("Accept"));

    QGridLayout *grid = new QGridLayout;
    dialog.setLayout(grid);

    grid->addWidget(heading, 1, 1, 1, 2);
    grid->addWidget(fileLabel, 2, 1);
    grid->addWidget(file, 2, 2);
    grid->addWidget(mimeLabel, 3, 1);
    grid->addWidget(mime, 3, 2);
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(cancel);
    hboxLayout->addWidget(accept);
    grid->addLayout(hboxLayout, 4, 2);

    cancel->setFlat(true);
    accept->setDefault(true);

    connect(cancel, &QPushButton::clicked, this, [item, &dialog] {
        item->cancel();
        dialog.reject();
    });

    connect(accept, &QPushButton::clicked, this, [this, item, &dialog] {
        item->accept();
        dialog.accept();

        addDownload(item);
    });

    Utils::setMovableByBackground(dialog.winId());

    dialog.exec();
}

void Downloads::addDownload(QWebEngineDownloadItem *item)
{
    QListWidgetItem *listItem = new QListWidgetItem;
    m_listWidget->addItem(listItem);

    QFileInfo info(item->path());

    QWidget *downloadWidget = new QWidget;
    QGridLayout *gridLayout = new QGridLayout;
    downloadWidget->setLayout(gridLayout);

    QProgressBar *progressBar = new QProgressBar;
    QToolButton *pauseResume = new QToolButton;
    QToolButton *cancel = new QToolButton;

    progressBar->setMinimum(0);
    progressBar->setMaximum(100);

    pauseResume->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-pause")));
    cancel->setIcon(QIcon::fromTheme(QStringLiteral("process-stop")));

    pauseResume->setAutoRaise(true);
    cancel->setAutoRaise(true);

    gridLayout->addWidget(new QLabel(info.fileName()), 1, 1, 1, 3);
    gridLayout->addWidget(progressBar, 2, 1);
    gridLayout->addWidget(pauseResume, 2, 2);
    gridLayout->addWidget(cancel, 2, 3);

    listItem->setSizeHint(downloadWidget->sizeHint());
    m_listWidget->setItemWidget(listItem, downloadWidget);

    connect(item, &QWebEngineDownloadItem::downloadProgress, [progressBar] (qint64 bytesRecieved, qint64 bytesTotal) {
        if (bytesTotal == 0) {
            progressBar->setValue(-1);
            return ;
        }

        int progress = bytesRecieved * 100 / bytesTotal;
        progressBar->setValue(progress);
    });

    connect(pauseResume, &QToolButton::clicked, [item, pauseResume] {
        if (item->isPaused()) {
            item->resume();
            pauseResume->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-pause")));
        } else {
            item->pause();
            pauseResume->setIcon(QIcon::fromTheme(QStringLiteral("media-playback-start")));
        }
    });

    connect(cancel, &QToolButton::clicked, [item] {
        item->cancel();
    });

    connect(item, &QWebEngineDownloadItem::stateChanged, [item, gridLayout, progressBar, pauseResume, cancel] {
        if (item->state() == QWebEngineDownloadItem::DownloadRequested || item->state() == QWebEngineDownloadItem::DownloadInProgress) {
            return ;
        }

        progressBar->setParent(nullptr);
        progressBar->deleteLater();

        pauseResume->setParent(nullptr);
        pauseResume->deleteLater();

        cancel->setParent(nullptr);
        cancel->deleteLater();

        QString description;
        switch (item->state()) {
        case QWebEngineDownloadItem::DownloadCompleted:
            description = QStringLiteral("Download completed");
            break;
        case QWebEngineDownloadItem::DownloadCancelled:
            description = QStringLiteral("Donwlaod cancelled");
            break;
        case QWebEngineDownloadItem::DownloadInterrupted:
            description = item->interruptReasonString();
        }

        gridLayout->addWidget(new QLabel(description), 2, 1, 1, 3);
    });
}
