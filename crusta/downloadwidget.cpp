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

#include "downloadwidget.h"

#include <QLabel>
#include <QUrl>
#include <QDesktopServices>
#include <QDir>
#include <QCoreApplication>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>



DownloadWidget::DownloadWidget(){
    cancel->setText(tr("Cancel"));
    cancel->setFixedWidth(75);
    progress->setMaximum(100);
    progress->setMinimum(0);
    setLayout(hbox);
    hbox->addWidget(icon);
    icon->setFixedWidth(30);
    hbox->addLayout(v0box);
    hbox->addLayout((v1box));
    v0box->addWidget(name);
    name->setFixedWidth(245);
    name->setAlignment(Qt::AlignLeft);
    v0box->addWidget(progress);
    v1box->addWidget(cancel);
    v1box->addWidget(fraction);
    setFixedWidth(350);
}

void DownloadWidget::getName(QString file){
    name->setText(file);
}

void DownloadWidget::getIcon(QIcon ico){
    icon->setPixmap(ico.pixmap(16,16));
}

void DownloadWidget::computeFraction(qint64 bytesRecieved, qint64 bytesTotal){
    int f=0;
    if(bytesTotal!=0)f=(int)((bytesRecieved*100)/bytesTotal);
    progress->setValue(f);
    float size;
    if(bytesTotal/1000.0<1){
        size=bytesTotal;
        fraction->setNum(size);
        fraction->setText(fraction->text()+"b");
    }
    else if(bytesTotal/1000000.0<1){
        size=bytesTotal/1000.0;
        fraction->setNum(size);
        fraction->setText(fraction->text()+"Kb");
    }
    else if(bytesTotal/1000000000.0<1){
        size=bytesTotal/1000000.0;
        fraction->setNum(size);
        fraction->setText(fraction->text()+"Mb");
    }
    else{
        size=bytesTotal/1000000000.0;
        fraction->setNum(size);
        fraction->setText(fraction->text()+"Gb");
    }
}

void DownloadWidget::changeLayout_Completed(){
    open->setText(tr("Open"));
    open->setFixedWidth(75);
    connect(open,&QPushButton::clicked,this,&DownloadWidget::openFile);
    remove->setText(tr("Remove"));
    remove->setFixedWidth(75);
    connect(remove,&QPushButton::clicked,this,&DownloadWidget::removeWidget);
    v0box->removeWidget(progress);
    v1box->removeWidget(fraction);
    v1box->removeWidget(cancel);
    progress->deleteLater();
    fraction->deleteLater();
    cancel->disconnect();
    cancel->deleteLater();
    v1box->addWidget(open);
    v1box->addWidget(remove);
    v0box->addWidget(new QLabel(tr("Completed ")+QDateTime::currentDateTime().toString()));
    QString forbidden=path;
    QFile f(QDir::homePath()+"/.crusta_db/downloads.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(line.split(">>>>>")[0]!=forbidden)
                s.append(line + "\n");
            else{
                s.append(path+">>>>>"+"Completed"+">>>>>"+QDateTime::currentDateTime().toString().toLatin1()+"\n");
            }
        }
        f.resize(0);
        t << s;
        f.close();
    }
}

void DownloadWidget::changeLayout_Canceled(){
    v0box->removeWidget(progress);
    v1box->removeWidget(fraction);
    v1box->removeWidget(cancel);
    progress->deleteLater();
    fraction->deleteLater();
    cancel->disconnect();
    cancel->deleteLater();
    v0box->addWidget(new QLabel(tr("Download Canceled ...")+QDateTime::currentDateTime().toString()));

    remove->setText(tr("Remove"));
    remove->setFixedWidth(75);
    connect(remove,&QPushButton::clicked,this,&DownloadWidget::removeWidget);
    v1box->addWidget(remove);

    QString forbidden=path;
    QFile f(QDir::homePath()+"/.crusta_db/downloads.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(line.split(">>>>>")[0]!=forbidden)
                s.append(line + "\n");
            else{
                s.append(path+">>>>>"+"Canceled"+">>>>>"+QDateTime::currentDateTime().toString().toLatin1()+"\n");
            }
        }
        f.resize(0);
        t << s;
        f.close();
    }
}

void DownloadWidget::changeLayout_Interrupted(){
    v0box->removeWidget(progress);
    v1box->removeWidget(fraction);
    v1box->removeWidget(cancel);
    progress->deleteLater();
    fraction->deleteLater();
    cancel->disconnect();
    cancel->deleteLater();
    v0box->addWidget(new QLabel(tr("Download Interrupted ...")+QDateTime::currentDateTime().toString()));

    remove->setText(tr("Remove"));
    remove->setFixedWidth(75);
    connect(remove,&QPushButton::clicked,this,&DownloadWidget::removeWidget);
    v1box->addWidget(remove);

    QString forbidden=path;
    QFile f(QDir::homePath()+"/.crusta_db/downloads.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(line.split(">>>>>")[0]!=forbidden)
                s.append(line + "\n");
            else{
                s.append(path+">>>>>"+"Interrupted"+">>>>>"+QDateTime::currentDateTime().toString().toLatin1()+"\n");
            }
        }
        f.resize(0);
        t << s;
        f.close();
    }
}

void DownloadWidget::stateChanged(QWebEngineDownloadItem::DownloadState state){
    switch(state){
    case QWebEngineDownloadItem::DownloadCompleted:
        this->changeLayout_Completed();
        return;
    case QWebEngineDownloadItem::DownloadCancelled:
        this->changeLayout_Canceled();
        return;
    default:
        this->changeLayout_Interrupted();
        return;
    }
}

void DownloadWidget::openFile(){
    QUrl u;
    path=path.split(">>>>>")[0];
    u=QUrl::fromLocalFile(path);
    QFile f(path);
    if(!f.exists()){
        QMessageBox msgBox;
        msgBox.setText("File not found");
        msgBox.setInformativeText(tr("the requested file not found. It might be replaced or deleted"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    QDesktopServices::openUrl(u);
}

void DownloadWidget::removeWidget(){
    item->setHidden(true);
    QString forbidden=path;
    QFile f(QDir::homePath()+"/.crusta_db/downloads.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(line.split(">>>>>")[0]!=forbidden.split(">>>>>")[0])
                s.append(line + "\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
}
