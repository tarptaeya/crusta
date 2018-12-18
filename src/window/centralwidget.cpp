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
#include "centralwidget.h"
#include "sidebar.h"
#include "tabwidget.h"
#include "layouts.h"

CentralWidget::CentralWidget(QWidget *parent)
    : QSplitter(parent)
{
    m_tabWidget = new TabWidget(this);
    addWidget(m_tabWidget);
    m_sideBar = new SideBar(this);
    if (Layouts::sideBarPosition() == Layouts::Left) {
        insertWidget(0, m_sideBar);
        setStretchFactor(0, 0);
        setStretchFactor(1, 1);
    } else {
        addWidget(m_sideBar);
        setStretchFactor(0, 1);
        setStretchFactor(1, 0);
    }

    setHandleWidth(0);
    setContentsMargins(0, 0, 0, 0);
}

CentralWidget::~CentralWidget()
{
}

TabWidget *CentralWidget::tabWidget()
{
    return m_tabWidget;
}
