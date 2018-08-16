#include "tabbarbutton.h"

TabBarButton::TabBarButton(QWidget *parent)
    : QPushButton(parent)
{
    int side = parent->height();
    setFixedSize(side, side);
    setFlat(true);
    setIconSize(QSize(24, 24));
}

QVariant TabBarButton::data() const
{
    return m_data;
}

void TabBarButton::setData(QVariant data)
{
    m_data = data;
}
