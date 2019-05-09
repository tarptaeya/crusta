#pragma once

#include <QObject>
#include <QDateTime>
#include <QTreeWidget>

struct HistoryItem
{
    QString title;
    QString url;
    QDateTime timestamp;
};

class History
{
public:
    static void insertItem(const HistoryItem &item);

    static QTreeWidget *historyWidget();
};
