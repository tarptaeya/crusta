#include "layouts.h"
#include "../bootstrap/appmanager.h"
#include "../bootstrap/settings.h"

Layouts::Layouts()
{
}

Layouts::Position Layouts::sideBarPosition()
{
    appManager->settings()->beginGroup(APPEARANCE);
    Position position = static_cast<Position>(appManager->settings()->value(QStringLiteral("sideBarPosition"), Left).toInt());
    appManager->settings()->endGroup();
    return position;
}
