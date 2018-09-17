#include "speeddialitem.h"

SpeeddialItem::SpeeddialItem()
{

}

QByteArray SpeeddialItem::image() const
{
    return m_image;
}

void SpeeddialItem::setImage(const QByteArray &image)
{
    m_image = image;
}
