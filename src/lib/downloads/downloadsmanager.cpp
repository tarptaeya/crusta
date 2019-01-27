#include "common-defs.h"
#include "downloaddialog.h"
#include "downloaditemwidget.h"
#include "downloadsmanager.h"
#include "mainapplication.h"

#include <QVBoxLayout>

DownloadsManager::DownloadsManager(QWidget *parent)
    : QWidget (parent)
{
    m_listWidget = new QListWidget(this);
    QVBoxLayout *vboxLayout = new QVBoxLayout;

    vboxLayout->addWidget(m_listWidget);

    setLayout(vboxLayout);
}

void DownloadsManager::downloadRequested(QWebEngineDownloadItem *item)
{
    if (appManager->settings()->value(QSL("downloadManager/askBeforeDownloading"), true).toBool()) {
        askBeforeDownloading(item);
        return;
    }

    downloadItem(item);
}

void DownloadsManager::askBeforeDownloading(QWebEngineDownloadItem *item)
{
    DownloadDialog dialog(item);
    if (dialog.exec() == QDialog::Accepted) {
        downloadItem(item, dialog.openAfterDownload());
        return;
    }

    item->cancel();
}

void DownloadsManager::downloadItem(QWebEngineDownloadItem *item, bool openAfterDownload)
{
    DownloadItemWidget *widget = new DownloadItemWidget(item, openAfterDownload);
    QListWidgetItem *listItem = new QListWidgetItem(m_listWidget);
    listItem->setSizeHint(widget->sizeHint());
    m_listWidget->insertItem(0, listItem);
    m_listWidget->setItemWidget(listItem, widget);

    item->accept();
}
