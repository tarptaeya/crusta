#include "historyitem.h"

HistoryItem::HistoryItem()
{
}

int HistoryItem::timestamp() const
{
    return m_timestamp;
}

void HistoryItem::setTimestamp(int timestamp)
{
    m_timestamp = timestamp;
}

QByteArray HistoryItem::favicon() const
{
    return m_favicon;
}

void HistoryItem::setFavicon(const QByteArray &favicon)
{
    m_favicon = favicon;
}

int HistoryItem::visitCount() const
{
    return m_visitCount;
}

void HistoryItem::setVisitCount(int visitCount)
{
    m_visitCount = visitCount;
}

int HistoryItem::loadingTime() const
{
    return m_loadingTime;
}

void HistoryItem::setLoadingTime(int loadingTime)
{
    m_loadingTime = loadingTime;
}
