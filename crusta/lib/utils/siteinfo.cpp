/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <anmol@crustabrowser.com>
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

#include "siteinfo.h"
#include <QWebEngineProfile>

SiteInfoWidget::SiteInfoWidget(QWebEngineView* view){
    setLayout(vbox);
    QHBoxLayout* h0box=new QHBoxLayout();
    QLabel* title=new QLabel(tr("Title"));
    title->setFixedWidth(100);
    h0box->addWidget(title);
    QLabel* dot0=new QLabel(":");
    dot0->setFixedWidth(10);
    h0box->addWidget(dot0);
    QLabel* title_str=new QLabel(view->title());
    title_str->setAlignment(Qt::AlignLeft);
    h0box->addWidget(title_str);
    h0box->addWidget(new QLabel());
    vbox->addLayout(h0box);
    QHBoxLayout* h1box=new QHBoxLayout();
    QLabel* url=new QLabel(tr("URL"));
    url->setFixedWidth(100);
    h1box->addWidget(url);
    QLabel* dot1=new QLabel(":");
    dot1->setFixedWidth(10);
    h1box->addWidget(dot1);
    QLabel* url_str=new QLabel(view->url().toString());
    url_str->setAlignment(Qt::AlignLeft);
    h1box->addWidget(url_str);
    vbox->addLayout(h1box);
    QHBoxLayout* h2box=new QHBoxLayout();
    QLabel* cachepath=new QLabel(tr("Cache Path"));
    cachepath->setFixedWidth(100);
    h2box->addWidget(cachepath);
    QLabel* cachepath_str=new QLabel(view->page()->profile()->cachePath());
    cachepath_str->setAlignment(Qt::AlignLeft);
    QLabel* dot2=new QLabel(":");
    dot2->setFixedWidth(10);
    h2box->addWidget(dot2);
    h2box->addWidget(cachepath_str);
    vbox->addLayout(h2box);
    QHBoxLayout* h3box=new QHBoxLayout();
    QLabel* storagename=new QLabel(tr("Storage Name"));
    storagename->setFixedWidth(100);
    h3box->addWidget(storagename);
    QLabel* dot3=new QLabel(":");
    dot3->setFixedWidth(10);
    h3box->addWidget(dot3);
    QLabel* storagename_str=new QLabel(view->page()->profile()->storageName());
    storagename_str->setAlignment(Qt::AlignLeft);
    h3box->addWidget(storagename_str);
    vbox->addLayout(h3box);
    QHBoxLayout* h4box=new QHBoxLayout();
    QLabel* type=new QLabel(tr("Connection"));
    type->setFixedWidth(100);
    h4box->addWidget(type);
    QLabel* dot4=new QLabel(":");
    dot4->setFixedWidth(10);
    h4box->addWidget(dot4);
    QLabel* type_str=new QLabel();
    if(view->url().toString().startsWith("https://")){
        type_str->setText(tr("Connection is Secure"));
    }
    else if(view->url().toString().startsWith("http://")){
        type_str->setText(tr("Connection is Not Secure"));
    }
    else{
        type_str->setText(tr("Unknown Security"));
    }
    type_str->setAlignment(Qt::AlignLeft);
    h4box->addWidget(type_str);
    vbox->addLayout(h4box);
    QHBoxLayout* h5box=new QHBoxLayout();
    h5box->addWidget(new QLabel());
    QPushButton* ok=new QPushButton();
    ok->setFixedWidth(100);
    ok->setDefault(false);
    h5box->addWidget(ok);
    h5box->addWidget(new QLabel());
    vbox->addLayout(h5box);
    ok->setText(tr("OK"));
    //setStyleSheet("QWidget{color:black;background-color:white} QPushButton{border:0.5px solid;background-color:black;color:white;border-radius:0px;border-color:black;padding:2px 4px;} QPushButton:hover{background-color:white;color:black}");
    connect(ok,&QPushButton::clicked,this,&QDialog::accept);
    setWindowTitle("Crusta : SiteInfo");
    setFixedSize(this->sizeHint());
}
