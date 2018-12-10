#pragma once

#include "abstractdataitem.h"

class PanelItem : public AbstractDataItem
{
public:
    explicit PanelItem();
    QByteArray favicon() const;
    void setFavicon(const QByteArray &favicon);

private:
    QByteArray m_favicon;
};
