/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <tarptaeya@gmail.com>
*
* THIS FILE IS A PART OF CRUSTA
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "dimensions.h"
#include "appmanager.h"
#include "settings.h"

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

QMargins Dimensions::lineEditMargins()
{
    return QMargins(10, 4, 10, 4);
}

int Dimensions::contextMenuTextLength()
{
    return 10;
}
