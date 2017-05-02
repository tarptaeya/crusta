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

#include "downloaditemwidget.h"
#include <QIcon>
#include <iostream>

DownloadItemWidget::DownloadItemWidget(){
    setFixedWidth(170);
    setLayout(hbox);
    hbox->addWidget(progress);
    hbox->addWidget(title);
    cancel_btn->setFlat(true);
    cancel_btn->setIcon(QIcon(":/res/drawables/close.png"));
    hbox->addWidget(cancel_btn);
}

void DownloadItemWidget::downloadProgress(qint64 bytesReceived, qint64 bytesTotal){
    qint64 p=0;
    if(item->totalBytes()!=qint64(0)){
        p=(bytesReceived*100)/item->totalBytes();
    }
    std::cout<<"downloading "<<bytesReceived<<" of "<<item->totalBytes()<<std::endl;
    progress->setText(QString::number(p)+QString("%"));
}

void DownloadItemWidget::setDownloadItem(QWebEngineDownloadItem *d_item){
    item=d_item;
}

