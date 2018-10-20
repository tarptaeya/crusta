#include "toolbarbutton.h"

ToolBarButton::ToolBarButton(QWidget *parent)
    : QToolButton(parent)
{
    setObjectName("tool-bar-button");
}

void ToolBarButton::setIconFromFileName(const QString &iconFileName)
{
    setIcon(QIcon(iconFileName));
    setIconSize(QSize(24, 24));
}
