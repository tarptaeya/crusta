#include "sidebar.h"
#include "../utils/strings.h"

SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(Strings::sideBarName());
}

SideBar::~SideBar()
{
}
