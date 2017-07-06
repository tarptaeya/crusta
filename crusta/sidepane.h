/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017 Anmol Gautam <anmol@crustabrowser.com>
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

#ifndef SIDEPANE_H
#define SIDEPANE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>


class SidePaneButton: public QPushButton{
public:
    SidePaneButton();
};


class SidePane: public QWidget{
public:
    SidePane();
    SidePaneButton* history=new SidePaneButton();
    SidePaneButton* bookmarks=new SidePaneButton();
    SidePaneButton* downloads=new SidePaneButton();
    SidePaneButton* add_pane_btn=new SidePaneButton();
};


#endif // SIDEPANE_H
