#ifndef CR_HISTORY_ITEM_H
#define CR_HISTORY_ITEM_H

#include <QDateTime>
#include <QString>
#include <QWebEngineHistoryItem>

class HistoryItem
{
public:
    QDateTime dateTime;
    QString title;
    QString url;

    static HistoryItem fromWebEngineHistoryItem(const QWebEngineHistoryItem &webEngineHistoryItem);
};

#endif
