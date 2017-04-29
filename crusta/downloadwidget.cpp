/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017 Anmol Gautam <tarptaeya@gmail.com>
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

#include "downloadwidget.h"
#include <QPropertyAnimation>

void DownloadWidget::createDownloadWidget(){
    setStyleSheet("#widget{border-top:1px solid grey;background-color:#ffffff}");
    setFixedHeight(50);
    setFixedWidth(parentView->geometry().width());
    setLayout(hbox);
    hbox->addWidget(hide_btn);
}

void DownloadWidget::setViewParent(QWebEngineView *view){
    parentView=view;
    setParent(view);
}

void DownloadWidget::showDownloadWidget(){
    show();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(600);
    animation->setStartValue(QPoint(0,parentView->geometry().height()));
    animation->setEndValue(QPoint(0,parentView->geometry().height()-50));
    animation->start();
}

void DownloadWidget::hideDownloadWidget(){
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(800);
    animation->setStartValue(QPoint(0,parentView->geometry().height()-50));
    animation->setEndValue(QPoint(0,parentView->geometry().height()));
    animation->start();
    setParent(0);
}
