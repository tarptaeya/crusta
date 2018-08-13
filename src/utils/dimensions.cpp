#include "dimensions.h"
#include "../bootstrap/appmanager.h"
#include "../bootstrap/settings.h"

int Dimensions::DEFAULT_SIDEBAR_WIDTH = 50;

Dimensions::Dimensions()
{
}

int Dimensions::sideBarWidth()
{
    appManager->settings()->beginGroup(APPEARANCE);
    int width = appManager->settings()->value(QStringLiteral("sideBarWidth"), DEFAULT_SIDEBAR_WIDTH).toInt();
    appManager->settings()->endGroup();
    return width;
}
