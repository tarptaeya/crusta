#pragma once

#include "abstractdataitem.h"
#include <QByteArray>

class HistoryItem : public AbstractDataItem
{
public:
    explicit HistoryItem();
    int timestamp() const;
    void setTimestamp(int timestamp);
    QByteArray favicon() const;
    void setFavicon(const QByteArray &favicon);
    int visitCount() const;
    void setVisitCount(int visitCount);
    int loadingTime() const;
    void setLoadingTime(int loadingTime);
private:
    int m_timestamp;
    QByteArray m_favicon;
    int m_visitCount;
    int m_loadingTime;
};
