#pragma once

#include <QObject>
#include <QUrl>

struct HistoryItem
{
    QString title;
    QUrl url;
    int count;
};

class History
{
};
