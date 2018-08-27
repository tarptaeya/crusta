#pragma once

#include <QString>
#include <QByteArray>

class HistoryItem
{
public:
    explicit HistoryItem();
    int timestamp() const;
    void setTimestamp(int timestamp);
    QByteArray favicon() const;
    void setFavicon(const QByteArray &favicon);
    QString title() const;
    void setTitle(const QString &title);
    QString url() const;
    void setUrl(const QString &url);
    int visitCount() const;
    void setVisitCount(int visitCount);
    int loadingTime() const;
    void setLoadingTime(int loadingTime);
private:
    int m_timestamp;
    QByteArray m_favicon;
    QString m_title;
    QString m_url;
    int m_visitCount;
    int m_loadingTime;
};
