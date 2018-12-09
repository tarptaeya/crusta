#include "downloadmanager.h"

QList<QWebEngineDownloadItem *> DownloadManager::s_downloadItems;

void DownloadManager::showPreDownloadDialog(QWebEngineDownloadItem *download)
{
    QDialog *dialog = new QDialog();
    dialog->setWindowTitle("Download Requested");
    QVBoxLayout *vbox = new QVBoxLayout;
    dialog->setLayout(vbox);

    QLabel *label = new QLabel();
    label->setText(QString("You Have requested to download\n"
                           "%1\n"
                           "which is of type %2\n"
                           "\n"
                           "What should Crusta do with this file?").arg(download->path(), download->mimeType()));
    vbox->addWidget(label);

    QHBoxLayout *hbox = new QHBoxLayout();
    QPushButton *saveButton = new QPushButton("Save");
    QPushButton *openButton = new QPushButton("Open");
    QPushButton *cancelButton = new QPushButton("Cancel");
    saveButton->setDefault(true);

    hbox->addWidget(cancelButton);
    hbox->addWidget(openButton);
    hbox->addWidget(saveButton);
    vbox->addLayout(hbox);

    connect(saveButton, &QPushButton::clicked, dialog, [dialog, download]{
        s_downloadItems.append(download);
        download->accept();
        dialog->accept();
    });

    connect(openButton, &QPushButton::clicked, dialog, [dialog, download]{
        s_downloadItems.append(download);
        download->accept();
        dialog->accept();
    });

    connect(cancelButton, &QPushButton::clicked, dialog, [dialog, download]{
        download->cancel();
        dialog->accept();
    });

    dialog->exec();

    dialog->deleteLater();
}
