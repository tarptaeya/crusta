#pragma once

#include "abstractdataitem.h"

class SpeeddialItem : public AbstractDataItem
{
public:
    explicit SpeeddialItem();
    QByteArray image() const;
    void setImage(const QByteArray &image);
private:
    QByteArray m_image;
};
