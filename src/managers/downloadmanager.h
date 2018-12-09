#pragma once

#include <QDialog>
#include <QWebEngineDownloadItem>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QString>

class DownloadManager : QObject {
    Q_OBJECT
public:
    static void showPreDownloadDialog(QWebEngineDownloadItem *download);
private:
    static QList<QWebEngineDownloadItem *> s_downloadItems;
};
