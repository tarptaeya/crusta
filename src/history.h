#pragma once

#include <QDateTime>
#include <QObject>

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

    static QWidget *historyWidget();
};
