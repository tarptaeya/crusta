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

DownloadWidget::DownloadWidget(){
    cancel->setText("Cancel");
    progress->setMaximum(100);
    progress->setMinimum(0);
    setLayout(hbox);
    hbox->addWidget(icon);
    hbox->addLayout(v0box);
    hbox->addLayout((v1box));
    v0box->addWidget(name);
    v0box->addWidget(progress);
    v1box->addWidget(cancel);
    v1box->addWidget(fraction);
}

void DownloadWidget::getName(QString file){
    name->setText(file);
}

void DownloadWidget::getIcon(QIcon ico){
    icon->setPixmap(ico.pixmap(64,64));
}

void DownloadWidget::computeFraction(qint64 bytesRecieved, qint64 bytesTotal){
    int f=0;
    if(bytesTotal!=0)f=(int)((bytesRecieved*100)/bytesTotal);
    fraction->setNum(f);
    progress->setValue(f);
}

void DownloadWidget::changeLayout(){
    open->setText(tr("Open"));
    remove->setText(tr("Remove"));
    v0box->removeWidget(progress);
    v1box->removeWidget(fraction);
    v1box->removeWidget(cancel);
    progress->deleteLater();
    fraction->deleteLater();
    cancel->disconnect();
    cancel->deleteLater();
    v1box->addWidget(open);
    v1box->addWidget(remove);
}
