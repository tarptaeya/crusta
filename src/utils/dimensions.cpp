#include "dimensions.h"
#include "../bootstrap/appmanager.h"
#include "../bootstrap/settings.h"

Dimensions::Dimensions()
{
}

int Dimensions::sideBarWidth()
{
    appManager->settings()->beginGroup(APPEARANCE);
    int width = appManager->settings()->value(QStringLiteral("sideBarWidth"), 50).toInt();
    appManager->settings()->endGroup();
    return width;
}

int Dimensions::tabBarHeight()
{
    appManager->settings()->beginGroup(APPEARANCE);
    int width = appManager->settings()->value(QStringLiteral("tabBarWidth"), 40).toInt();
    appManager->settings()->endGroup();
    return width;
}

int Dimensions::tabListWidth()
{
    return 250;
}

int Dimensions::tabListItemHeight()
{
    return 30;
}

int Dimensions::onmibarHeightOffsetFromParent()
{
    return 10;
}

QMargins Dimensions::omnibarMargins()
{
    return QMargins(10, 4, 10, 4);
}

int Dimensions::contextMenuTextLength()
{
    return 10;
}
