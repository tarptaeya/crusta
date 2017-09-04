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
#include <QLabel>

StatusBar::StatusBar(){
    setLayout(hbox);
    hbox->addWidget(new QLabel());
    hbox->setContentsMargins(0,0,0,0);
    hbox->addWidget(pbar);
    hbox->addWidget(resetzoombtn);
    resetzoombtn->setFixedWidth(50);
    hbox->addWidget(zoomslider);
    zoomslider->setFixedWidth(170);
    zoomslider->setFixedHeight(10);
    zoomslider->setOrientation(Qt::Horizontal);
    zoomslider->setValue(100);
    zoomslider->setMinimum(10);
    zoomslider->setMaximum(300);
    setFixedHeight(20);
}
