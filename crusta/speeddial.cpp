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

#include "speeddial.h"
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QDir>
#include <QStringList>
#include <QCoreApplication>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QDialog>
#include <QPixmap>
#include <QUrl>
#include <QWebEngineView>

#include <iostream>

SpeedDial::SpeedDial(){
    setWindowTitle("Crusta : Speed Dial");
    QVBoxLayout* vbox=new QVBoxLayout();
    setLayout(vbox);
    QHBoxLayout* h1box=new QHBoxLayout();
    QLabel* bg_image=new QLabel(tr("Background image"));
    bg_image->setFixedWidth(170);
    h1box->addWidget(bg_image);
    h1box->addWidget(bgimage);
    QPushButton* choose_bgimage=new QPushButton(tr("Choose Image"));
    h1box->addWidget(choose_bgimage);
    vbox->addLayout(h1box);
    vbox->addWidget(list);
    load();
    QPushButton* add_=new QPushButton(tr("Add"));
    QPushButton* remove_=new QPushButton(tr("Remove"));
    QPushButton* ok=new QPushButton(tr("OK"));
    QHBoxLayout* h2box=new QHBoxLayout();
    h2box->addWidget(new QLabel());
    h2box->addWidget(add_);
    h2box->addWidget(remove_);
    h2box->addWidget(ok);
    vbox->addLayout(h2box);
    connect(add_,&QPushButton::clicked,this,&SpeedDial::add);
    connect(remove_,&QPushButton::clicked,this,&SpeedDial::remove);
    connect(ok,&QPushButton::clicked,this,[this]{save();close();});
    QFileDialog* f=new QFileDialog();
    connect(choose_bgimage,&QPushButton::clicked,this,[this,f]{this->bgimage->setText(f->getOpenFileName());});
}

void SpeedDial::save(){
    if(bgimage->text()=="default"){
        bgimage->setText(QDir::homePath()+"/.crusta_db"+"/web/img/default.jpg");
    }
    QString upper="<!DOCTYPE html><html><head><title>New Tab</title><link rel=\"shortcut icon\" href=\"../crusta/res/drawables/icon_3.ico\"><style type=\"text/css\">"
         "body {background-image:url(\""+bgimage->text()+"\");background-repeat: norepeat}.box {width: 160px;height: 100px;border: none;}"
         ".box:hover {box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);}"
         ".entry {outline: none;margin-top: 100px;padding: 10px 20px;width: 100%;font-size: 20px;}</style></head><body>"
         "<div style=\"text-align: center;padding: 100px;width: 70%;margin: auto;\">";
    QString lower="</div></body></html>";
    QString middle="";
    QFile inputFile(QDir::homePath()+"/.crusta_db/speeddial.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(!(data[0]=="" || data[1]=="")){
              QString s="<a href=\""+data[1]+"\"><img src=\"img/"+data[0]+".png\" class=\"box\" style=\"margin: 25px\"></img></a>";
              middle+=s;
          }
       }
       inputFile.close();
    }
    QFile f(QDir::homePath()+"/.crusta_db"+"/web/index.html");
    if(f.open(QIODevice::WriteOnly))
    {
        QString s=upper+middle+lower;
        QTextStream t(&f);
        f.resize(0);
        t << s;
        f.close();
    }
    QFile fi(QDir::homePath()+"/.crusta_db/speeddial.txt");
    if(fi.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&fi);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(!line.startsWith(">>>>>"))
                s.append(line + "\n");
        }
        fi.resize(0);
        t << ">>>>>"+bgimage->text().toLatin1()+"\n"+s;
        fi.close();
    }
}


void SpeedDial::load(){
    QFile inputFile(QDir::homePath()+"/.crusta_db/speeddial.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data[0]==""){
              bgimage->setText(data[1]);
          }
          if(!(data[0]=="" || data[1]=="")){
              list->addItem(data[0]);
          }
       }
       inputFile.close();
    }
    if(bgimage->text()==QDir::homePath()+"/.crusta_db"+"/web/img/default.jpg"){
        bgimage->setText("default");
    }
}

void SpeedDial::add(){
    QDialog* dg=new QDialog(this);
    QLineEdit* data0=new QLineEdit();
    QLineEdit* data1=new QLineEdit();
    data0->setPlaceholderText(tr("Title"));
    data1->setPlaceholderText("URL");
    QPushButton* ok=new QPushButton(tr("OK"));
    QPushButton* cncl=new QPushButton(tr("Cancel"));
    QVBoxLayout* vbox=new QVBoxLayout();
    dg->setLayout(vbox);
    vbox->addWidget(data0);
    vbox->addWidget(data1);
    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(ok);
    hbox->addWidget(cncl);
    vbox->addLayout(hbox);
    connect(ok,&QPushButton::clicked,dg,&QDialog::accept);
    connect(cncl,&QPushButton::clicked,dg,&QDialog::reject);
    if(dg->exec()!=QDialog::Accepted){
        return;
    }
    QWebEngineView* view=new QWebEngineView();
    view->load(QUrl(data1->text()));
    view->setWindowTitle("Processing thumbnail for Speed-Dial... Please Wait");
    view->show();
    connect(view,&QWebEngineView::loadFinished,view,[data0,view]{view->grab().save(QDir::homePath()+"/.crusta_db"+"/web/img/"+data0->text()+".png");view->close();});
    list->addItem(data0->text());
    QFile f(QDir::homePath()+"/.crusta_db/speeddial.txt");
    if(f.open(QIODevice::Append))
    {
        QTextStream t(&f);
        t << data0->text()+">>>>>"+data1->text()+"\n";
        f.close();
    }
    save();
}

void SpeedDial::remove(){
    QString forbidden=list->currentItem()->text();
    QDir img;
    img.remove(QDir::homePath()+"/.crusta_db"+"/web/img/"+forbidden+".png");
    QFile f(QDir::homePath()+"/.crusta_db/speeddial.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(line.split(">>>>>")[0]!=forbidden)
                s.append(line + "\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
    list->clear();
    load();
    save();
}

void SpeedDial::configure(){
    QString a=QCoreApplication::applicationDirPath()+"/web/img/";
    QString b=QDir::homePath()+"/.crusta_db/web/img/";
    QDir d(QCoreApplication::applicationDirPath()+"/web/img/");
    QStringList filesList = d.entryList(QStringList("*"));
    for(QString file:filesList){
        QFile::copy(a+file,b+file);
    }
}
