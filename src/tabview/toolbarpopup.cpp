#include "toolbarpopup.h"

ToolBarPopup::ToolBarPopup(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlag(Qt::Popup);
}
