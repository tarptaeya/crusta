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
#include "tabbar.h"
#include "tabwidget.h"

#define MAX_TAB_WIDTH 300
#define MIN_TAB_WIDTH 75
#define AUX_WIDTH 50

TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
    setDocumentMode(true);
    setMovable(true);
    setTabsClosable(true);
    setUsesScrollButtons(true);
}

QSize TabBar::tabSizeHint(int index) const
{
    int count = this->count() ? this->count() : 1;
    int width = (this->size().width() - AUX_WIDTH) / count;
    if (width > MAX_TAB_WIDTH) {
        width = MAX_TAB_WIDTH;
    }
    if (width < MIN_TAB_WIDTH) {
        width = MIN_TAB_WIDTH;
    }
    QSize size = QTabBar::tabSizeHint(index);
    size.setWidth(width);
    return size;
}
