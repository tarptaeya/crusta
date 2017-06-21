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

#include "featurenotifier.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QWebEngineView>
#include <QTimer>
#include <QString>
#include <QLabel>
#include <QObject>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

void FeatureNotifier::createNotifier(QString text){
    this->label->setStyleSheet("background:#ffffff; width:500px;height:20px");
    this->label->setText(text);
}

void FeatureNotifier::showNotifier(){
    this->label->setGeometry(0,0,500,80);
    this->label->setAlignment(Qt::AlignCenter);
    this->label->move(this->x,60);
    this->label->setStyleSheet("border: 3px solid gray;border-radius:20px;background-color:#ffffff;color:gray");
    QGraphicsOpacityEffect* effect=new QGraphicsOpacityEffect();
    this->label->setGraphicsEffect(effect);
    QPropertyAnimation* a=new QPropertyAnimation(effect,"opacity");
    a->setDuration(1000);
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    this->label->show();
    connect(this->timer,&QTimer::timeout,this,&FeatureNotifier::fadeOut);
    this->timer->start(3000);
}

void FeatureNotifier::fadeOut(){
    this->label->setParent(0);
    this->label->hide();
//    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
//    this->label->setGraphicsEffect(effect);
//    QPropertyAnimation *a = new QPropertyAnimation(effect,"opacity");
//    a->setDuration(1000);
//    a->setStartValue(1);
//    a->setEndValue(0);
//    a->setEasingCurve(QEasingCurve::OutBack);
//    a->start(QPropertyAnimation::DeleteWhenStopped);
//    connect(a,SIGNAL(finished()),this->label,SLOT(hide()));
}

void FeatureNotifier::setViewParent(QWebEngineView *view){
    this->label->setParent(view);
    this->x=QApplication::desktop()->screenGeometry().width()/2-190;
}

void FeatureNotifier::hideNotifier(){
    this->label->hide();
}
