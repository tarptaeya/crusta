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

#include "statusbar.h"
#include <QSettings>

StatusBar::StatusBar(SidePane *pane){
    setFixedHeight(20);
    box->setMargin(0);
    box->setSpacing(0);
    box->setContentsMargins(0,0,0,0);
    box->addWidget(toggle_btn);
    toggle_btn->setIcon(QIcon(":/res/drawables/togglepane.png"));
    toggle_btn->setFixedSize(40,20);
    connect(toggle_btn, &QPushButton::clicked, this, [this, pane]{
        if(pane->isVisible()){
            QSettings("Tarptaeya", "Crusta").setValue("sidepanel_visibility", 0);
            pane->hide();
        }
        else{
            QSettings("Tarptaeya", "Crusta").setValue("sidepanel_visibility", 1);
            pane->show();
        }
    });
    box->addWidget(link_lbl);
    box->addWidget(new QLabel());
    setStyleSheet("QPushButton{border: none} QPushButton:hover{background-color: #d0d0d0}");
    setLayout(box);
}
