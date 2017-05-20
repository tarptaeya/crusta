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

#include "tabwindow.h"
#include "webview.h"
#include "privatewebview.h"

#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QString>
#include <QWebEngineProfile>

#include <iostream>



void TabWindow::viewHome(){
    this->view->returnView()->load(QUrl(this->view->home_page));
}

void TabWindow::updateAddrBar(){
    this->addr_bar->initialize()->setText(this->view->returnView()->url().toString());
    this->addr_bar->initialize()->setCursorPosition(0);
    QString s=this->addr_bar->text();
    QFile inputFile("completer.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if(line==s)
              return;
       }
       inputFile.close();
    }
    QFile file("completer.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out<<s.toLatin1()+"\n";
    file.close();
}

void TabWindow::createControls(){
    QHBoxLayout* hbox=new QHBoxLayout();
    this->back_btn->setFlat(true);
    this->back_btn->setIcon(QIcon(":/res/drawables/back.svg"));
    connect(this->back_btn,&QPushButton::clicked,this->view->returnView(),&QWebEngineView::back);
    hbox->addWidget(this->back_btn);
    this->fwd_btn->setFlat(true);
    this->fwd_btn->setIcon(QIcon(":/res/drawables/forward.svg"));
    connect(this->fwd_btn,&QPushButton::clicked,this->view->returnView(),&QWebEngineView::forward);
    hbox->addWidget(this->fwd_btn);
    this->load_btn->setFlat(true);
    this->load_btn->setIcon(QIcon(":/res/drawables/reload.svg"));
    connect(this->load_btn,&QPushButton::clicked,this->view->returnView(),&QWebEngineView::reload);
    hbox->addWidget(this->load_btn);
    hbox->addWidget(this->addr_bar->initialize());
    connect(this->view->returnView(),&QWebEngineView::urlChanged,this,&TabWindow::updateAddrBar);
    connect(this->view->returnView(),&QWebEngineView::loadFinished,this,&TabWindow::updateStar);
    connect(this->addr_bar->initialize(),&QLineEdit::returnPressed,this,&TabWindow::loadUrl);
    //hbox->addWidget(this->search_bar->initialize());
    this->home_btn->setFlat(true);
    this->home_btn->setIcon(QIcon(":/res/drawables/home.svg"));
    connect(this->home_btn,&QPushButton::clicked,this,&TabWindow::viewHome);
    hbox->addWidget(this->home_btn);
    this->bookmark_btn->setFlat(true);
    this->bookmark_btn->setIcon(QIcon(":/res/drawables/bookmark.svg"));
    connect(this->bookmark_btn,&QPushButton::clicked,this,&TabWindow::bookmarkPage);
    hbox->addWidget(this->bookmark_btn);
    vbox->addLayout(hbox);
    vbox->addWidget(view);
    tab->setLayout(vbox);
    tab->setStyleSheet("QWidget{background-color:white;color:blueviolet;} QLineEdit{border:0.5px solid blueviolet;border-radius:10px;} QMenu::item:selected{color:white;background-color:blueviolet}");
}

QWidget* TabWindow::returnTab(){
    this->vbox->setContentsMargins(0,0,0,0);
    createControls();
    return this->tab;
}

TabWindow* TabWindow::returnThis(){
    this->vbox->setContentsMargins(0,0,0,0);
    createControls();
    return this;
}

void TabWindow::setWebView(WebView* v){
    view=v;
}

QWidget* TabWindow::returnTab(WebView* view){
    this->vbox->setContentsMargins(0,0,0,0);
    setWebView(view);
    createControls();
    return this->tab;
}

void TabWindow::loadUrl(){
    QString text=this->addr_bar->initialize()->text();
    QStringList textList=text.split(" ");
    if(textList.length()==1){
        if(text.startsWith("crusta://")){
            this->view->returnView()->load(QUrl(text));
        }
        else if(text.startsWith("localhost:")||text=="localhost"){
            this->view->returnView()->load(QUrl("http://"+text));
        }
        else if(text.startsWith("http://localhost")||text.startsWith("http://localhost:")){
            this->view->returnView()->load(QUrl(text));
        }
        else if(text.split('.').length()==1){
            QString searchStr=this->addr_bar->defaultSearch+text;
            this->view->returnView()->load(QUrl(searchStr));
        }
        else{
            if(!(text.startsWith("http://")||text.startsWith("https://"))){
                text="http://"+text;
            }
            this->view->returnView()->load(QUrl(text));
        }
    }
    else{
        QString searchStr=this->addr_bar->defaultSearch+text;
        this->view->returnView()->load(QUrl(searchStr));
    }
}

void TabWindow::bookmarkPage(){
    QFile file("bookmarks.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out << this->view->returnView()->title().toLatin1()+">>>>>"+this->view->returnView()->url().toString().toLatin1()+">>>>>"+"\n";
    file.close();
    this->bookmark_btn->setIcon(QIcon(":/res/drawables/star.svg"));
}

void TabWindow::updateStar(){
    QString s=this->addr_bar->text();
    QFile input("bookmarks.txt");
    if (input.open(QIODevice::ReadOnly))
    {
       QTextStream in(&input);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data.count()==1)continue;
          if(data.count()==2)data.append("");
          if(data[1]==s){
              this->bookmark_btn->setIcon(QIcon(":/res/drawables/star.svg"));
              input.close();
              return;
          }
       }
       input.close();
    }
    this->bookmark_btn->setIcon(QIcon(":/res/drawables/bookmark.svg"));
}
