#pragma once

#include <QListWidget>
#include <QWebEngineDownloadItem>
#include <QWidget>

class DownloadWidget : public QWidget
{
    QListWidget *m_list_widget = nullptr;
public:
    explicit DownloadWidget(QWidget *parent = nullptr);

    void handle_download(QWebEngineDownloadItem *item);
};
