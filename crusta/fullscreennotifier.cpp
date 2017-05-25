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

#include "fullscreennotifier.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QWebEngineView>
#include <QTimer>
#include <QLabel>
#include <QObject>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

void FullScreenNotifier::createNotifier(){
    this->label->setStyleSheet("background:#ffffff; width:500px;height:20px");
    this->label->setText(tr("Crusta is now fullscreen - press ESC to exit fullscreen"));
}

void FullScreenNotifier::showNotifier(){
    createNotifier();
    this->label->setGeometry(0,0,380,80);
    this->label->setAlignment(Qt::AlignCenter);
    this->label->move(this->x,60);
    this->label->setStyleSheet("border: 3px solid black;border-radius:20px;background-color:white;color:black");
    QGraphicsOpacityEffect* effect=new QGraphicsOpacityEffect();
    this->label->setGraphicsEffect(effect);
    QPropertyAnimation* a=new QPropertyAnimation(effect,"opacity");
    a->setDuration(1000);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    this->label->show();
    connect(this->timer,&QTimer::timeout,this,&FullScreenNotifier::fadeOut);
    this->timer->start(4200);
}

void FullScreenNotifier::fadeOut(){
    this->label->setParent(0);
    this->label->hide();
}

void FullScreenNotifier::setViewParent(QWebEngineView *view){
    this->label->setParent(view);
    this->x=QApplication::desktop()->screenGeometry().width()/2-190;
}

void FullScreenNotifier::hideNotifier(){
    this->label->hide();
}
