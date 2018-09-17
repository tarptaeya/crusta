#pragma once

#include <QString>

class AbstractDataItem
{
public:
    explicit AbstractDataItem();
    QString title() const;
    void setTitle(const QString &title);
    QString url() const;
    void setUrl(const QString &url);
private:
    QString m_title;
    QString m_url;
};
