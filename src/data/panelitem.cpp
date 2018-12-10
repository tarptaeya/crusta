#include "panelitem.h"

PanelItem::PanelItem()
    : AbstractDataItem()
{
}

QByteArray PanelItem::favicon() const
{
    return m_favicon;
}

void PanelItem::setFavicon(const QByteArray &favicon)
{
    m_favicon = favicon;
}
