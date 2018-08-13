#include "sidebar.h"
#include "../utils/strings.h"
#include "../utils/dimensions.h"
#include <QDebug>

SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(Strings::sideBarName());
    setFixedWidth(Dimensions::sideBarWidth());
}

SideBar::~SideBar()
{
}
