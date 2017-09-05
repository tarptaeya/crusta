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
#include <QSound>
#include <QWebEnginePage>

StatusBar::StatusBar(){
    setLayout(hbox);
    hbox->addWidget(hoveredlink);
    hbox->setContentsMargins(0,0,0,0);
    hbox->addWidget(pbar);
    QPushButton* capturebtn=new QPushButton();
    hbox->addWidget(capturebtn);
    capturebtn->setFixedWidth(20);
    capturebtn->setIcon(QIcon(":/res/drawables/status_shutter.svg"));
    connect(capturebtn,&QPushButton::clicked,this,[this]{
       QPixmap scrshot = view->grab();
       QSound::play(":/res/audio/shutter.wav");
       scrshot.save("c:/users/hp/desktop/"+view->title()+".png");
    });
    hbox->addWidget(resetzoombtn);
    connect(resetzoombtn,&QPushButton::clicked,this,[this]{
        view->setZoomFactor(1);
        zoomslider->setValue(100);
    });
    resetzoombtn->setFixedWidth(50);
    hbox->addWidget(zoomslider);
    hbox->addWidget(zoomindicator);
    QLabel* percentLabel=new QLabel("%");
    percentLabel->setFixedWidth(15);
    hbox->addWidget(percentLabel);
    zoomindicator->setNum(100);
    zoomindicator->setFixedWidth(20);
    zoomslider->setFixedWidth(170);
    zoomslider->setFixedHeight(10);
    zoomslider->setOrientation(Qt::Horizontal);
    zoomslider->setValue(100);
    zoomslider->setMinimum(10);
    zoomslider->setMaximum(300);
    connect(zoomslider,&QSlider::valueChanged,this,&StatusBar::changeZoom);
    setFixedHeight(20);
    setStyleSheet("QPushButton{background-color: #f0f0f0} QPushButton::hover{background-color: #d0d0d0}");
}

void StatusBar::getWebview(QWebEngineView *mview){
    view=mview;
    connect(view->page(),&QWebEnginePage::linkHovered,this,&StatusBar::showLinkHovered);
}

void StatusBar::changeZoom(int value){
    view->setZoomFactor(value/100.0);
    zoomindicator->setNum(value);
}

void StatusBar::showLinkHovered(const QString &url){
    QString text(url);
    QFontMetrics metrics(hoveredlink->font());
    QString elidedText = metrics.elidedText(text, Qt::ElideRight, hoveredlink->width()-10);
    hoveredlink->setText(elidedText);
}
