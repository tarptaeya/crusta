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

#include <debugger.h>
#include <QPropertyAnimation>
#include <QPoint>

#include <iostream>

void WebDebugger::showDebugger(){
    show();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(600);
    start_point=parentView->geometry().height();
    animation->setStartValue(QPoint(0,start_point));
    animation->setEndValue(QPoint(0,start_point-400));
    animation->start();
    std::cout<<"Showed"<<std::endl;
    connect(this,&WebDebugger::urlChanged,this,&WebDebugger::print);
}

void WebDebugger::print(QUrl url){
    std::cout<<url.toString().toStdString()<<std::endl;
}

void WebDebugger::setParentView(MainView *view){
    parentView=view->window;
    setParent(parentView);
    load(QUrl("http://localhost:8000"));
}

void WebDebugger::hideDebugger(){
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");
    animation->setDuration(800);
    animation->setStartValue(QPoint(0,this->start_point-50));
    animation->setEndValue(QPoint(0,this->start_point));
    animation->start();
}
