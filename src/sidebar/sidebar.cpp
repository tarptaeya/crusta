#include "sidebar.h"
#include "../utils/strings.h"
#include "../utils/dimensions.h"
#include <QDebug>

SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setObjectName(Strings::sideBarName());
    setFixedWidth(Dimensions::sideBarWidth());
}

SideBar::~SideBar()
{
}
