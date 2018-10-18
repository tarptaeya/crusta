#include "addtabbutton.h"

AddTabButton::AddTabButton(QWidget *parent)
    : QPushButton(parent)
{
}

void AddTabButton::setSide(int side)
{
    setFixedSize(side, side);
}
