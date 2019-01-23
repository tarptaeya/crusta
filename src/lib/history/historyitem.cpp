#include "historyitem.h"

HistoryItem HistoryItem::fromWebEngineHistoryItem(const QWebEngineHistoryItem &webEngineHistoryItem)
{
    HistoryItem item;
    item.dateTime = webEngineHistoryItem.lastVisited();
    item.title = webEngineHistoryItem.title();
    item.url = webEngineHistoryItem.url().toDisplayString(QUrl::RemoveQuery);
    return item;
}
