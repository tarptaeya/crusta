#include "toolbarpopup.h"

ToolBarPopup::ToolBarPopup(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlag(Qt::Popup);
}

void ToolBarPopup::setParentAction(QAction *action)
{
    m_parentAction = action;
}

QAction *ToolBarPopup::parentAction()
{
    return m_parentAction;
}
