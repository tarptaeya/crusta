#ifndef CR_DOWNLOAD_DIALOG_H
#define CR_DOWNLOAD_DIALOG_H

#include <QDialog>
#include <QRadioButton>
#include <QWebEngineDownloadItem>

class DownloadDialog : public QDialog
{
public:
    explicit DownloadDialog(QWebEngineDownloadItem *item, QWidget *parent = nullptr);

    bool openAfterDownload();
private:
    QRadioButton *m_openAfterDownload = nullptr;
};

#endif
