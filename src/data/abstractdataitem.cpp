#include "abstractdataitem.h"

AbstractDataItem::AbstractDataItem()
{

}

QString AbstractDataItem::title() const
{
    return m_title;
}

void AbstractDataItem::setTitle(const QString &title)
{
    m_title = title;
}

QString AbstractDataItem::url() const
{
    return m_url;
}

void AbstractDataItem::setUrl(const QString &url)
{
    m_url = url;
}
