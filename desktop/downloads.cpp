#include "downloads.h"

#include <QDialog>
#include <QDir>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include <QUrl>

const int FIXED_WIDTH = 400;

DownloadWidget::DownloadWidget(QWidget *parent)
    : QWidget(parent)
{
    m_list_widget = new QListWidget;

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0, 0, 0, 0);
    setLayout(vbox);
    vbox->addWidget(m_list_widget);

    setMinimumWidth(FIXED_WIDTH);
}

void DownloadWidget::handle_download(QWebEngineDownloadItem *item)
{
    QDialog d(this);
    d.setWindowTitle(QStringLiteral("Download Requested"));
    d.setModal(true);
    {
        QVBoxLayout *vbox = new QVBoxLayout;
        d.setLayout(vbox);

        QLabel *label = new QLabel(QStringLiteral("Download %1?").arg(item->url().toString()));
        label->setWordWrap(true);
        vbox->addWidget(label);

        QLabel *file_type = new QLabel(QStringLiteral("Mime type: %1").arg(item->mimeType()));
        vbox->addWidget(file_type);

        qint64 total_bytes = item->totalBytes();
        QLabel *download_size = new QLabel(QStringLiteral("Download size: %1").arg(locale().formattedDataSize(total_bytes)));
        if (total_bytes == -1) {
            download_size->setText(QStringLiteral("Download size: unknown"));
        }
        vbox->addWidget(download_size);

        QPushButton *ok = new QPushButton(QStringLiteral("Allow download"));
        QPushButton *cancel = new QPushButton(QStringLiteral("Cancel"));
        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->addWidget(ok);
        hbox->addWidget(cancel);
        vbox->addLayout(hbox);

        connect(ok, &QPushButton::clicked, &d, &QDialog::accept);
        connect(cancel, &QPushButton::clicked, &d, &QDialog::reject);
    }

    QSettings settings;
    bool ask_for_downloads = settings.value(QStringLiteral("downloads/ask"), true).toBool();

    if (ask_for_downloads && d.exec() != QDialog::Accepted) {
        item->cancel();
        return;
    }

    item->accept();

    QWidget *widget = new QWidget;
    QVBoxLayout *vbox = new QVBoxLayout;
    widget->setLayout(vbox);

    widget->setFixedWidth(FIXED_WIDTH);

    QLabel *name = new QLabel(item->downloadFileName());
    vbox->addWidget(name);

    QHBoxLayout *hbox = new QHBoxLayout;
    vbox->addLayout(hbox);

    QProgressBar *progress = new QProgressBar;
    progress->setRange(0, 100);
    connect(item, &QWebEngineDownloadItem::downloadProgress, [progress] (qint64 bytes_received, qint64 bytes_total) {
        if (bytes_total == 0) {
            progress->setValue(-1);
            return ;
        }
        progress->setValue(bytes_received * 100 / bytes_total);
    });
    hbox->addWidget(progress, 1);

    QPushButton *cancel = new QPushButton(QStringLiteral("Cancel"));
    connect(cancel, &QPushButton::clicked, item, &QWebEngineDownloadItem::cancel);
    hbox->addWidget(cancel);

    QPushButton *pause = new QPushButton(QStringLiteral("Pause"));
    connect(pause, &QPushButton::clicked, [item, pause] {
        if (item->isPaused()) {
            item->resume();
            pause->setText(QStringLiteral("Pause"));
        } else {
            item->pause();
            pause->setText(QStringLiteral("Resume"));
        }
    });
    hbox->addWidget(pause);

    connect(item, &QWebEngineDownloadItem::finished, [item, hbox, progress, cancel, pause] {
        hbox->removeWidget(progress);
        hbox->removeWidget(cancel);
        hbox->removeWidget(pause);

        delete progress;
        delete cancel;
        delete pause;

        QLabel *label = new QLabel;
        QFont font;
        font.setPointSize(10);
        hbox->addWidget(label);
        switch (item->state()) {
        case QWebEngineDownloadItem::DownloadCompleted:
            label->setText(QStringLiteral("Downloaded."));
            break;
        case QWebEngineDownloadItem::DownloadCancelled:
            label->setText(QStringLiteral("Donwload Cancelled."));
            break;
        case QWebEngineDownloadItem::DownloadInterrupted:
            label->setText(item->interruptReasonString());
            break;
        default:
            break;
        }
    });

    QListWidgetItem *i = new QListWidgetItem;
    m_list_widget->insertItem(0, i);
    i->setSizeHint(widget->sizeHint());
    m_list_widget->setItemWidget(i, widget);
    m_list_widget->setMinimumWidth(widget->minimumWidth());
}
