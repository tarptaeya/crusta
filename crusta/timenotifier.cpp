#include "timenotifier.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QWebEngineView>
#include <QAction>
#include <QTime>
#include <QTimer>
#include <QLabel>
#include <QObject>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <Qt>

void TimeNotifier::createNotifier(){
    this->label->setStyleSheet("background:#ffffff; width:500px;height:20px");
    this->label->setText(QString("Current time : ")+QTime::currentTime().toString());
}

void TimeNotifier::showNotifier(){
    createNotifier();
    this->label->setGeometry(0,0,380,80);
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
    connect(this->timer,&QTimer::timeout,this,&TimeNotifier::fadeOut);
    this->timer->start(3000);
}

void TimeNotifier::fadeOut(){
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    this->label->setGraphicsEffect(effect);
    QPropertyAnimation *a = new QPropertyAnimation(effect,"opacity");
    a->setDuration(1000);
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a,SIGNAL(finished()),this->label,SLOT(hide()));
}

void TimeNotifier::setViewParent(QWebEngineView *view){
    this->label->setParent(view);
    this->x=QApplication::desktop()->screenGeometry().width()/2-190;
}

void TimeNotifier::hideNotifier(){
    this->label->hide();
}
