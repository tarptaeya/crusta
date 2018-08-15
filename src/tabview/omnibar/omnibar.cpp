#include "omnibar.h"

OmniBar::OmniBar(QWidget *parent)
    : QLineEdit(parent)
{
    int height = parent->height() - 10;
    setFixedHeight(height);
    setAttribute(Qt::WA_MacShowFocusRect, 0);
}
