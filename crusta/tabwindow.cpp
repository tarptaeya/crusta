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
#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QMenu>
#include <QDir>

#include <iostream>



void TabWindow::viewHome(){
    this->view->returnView()->load(QUrl(this->view->home_page));
}

void TabWindow::updateAddrBar(){
    QString url=this->view->returnView()->url().toString();
    QDir* exec_dir=new QDir(QCoreApplication::applicationDirPath());
    exec_dir->cd("../web");
    QString forbidden="file://"+exec_dir->absolutePath()+"/index.html";
    if(forbidden==url){
        return;
    }
    QDir* viewer_file=new QDir(QCoreApplication::applicationDirPath());
    viewer_file->cd("../3rd_party/pdfjs/web");
    QString forbidden_="file://"+viewer_file->absolutePath()+"/viewer.html?file";
    if(forbidden_==url.split("=")[0]){
        this->addr_bar->initialize()->setText(url.split("/")[url.split("/").length()-1]);
        this->addr_bar->initialize()->setCursorPosition(0);
        return;
    }
    this->addr_bar->initialize()->setText(url);
    this->addr_bar->initialize()->setCursorPosition(0);
    QString s=this->addr_bar->text();
    QFile inputFile(QDir::homePath()+"/.crusta_db/completer.txt");
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
    QFile file(QDir::homePath()+"/.crusta_db/completer.txt");
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
    this->home_btn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->home_btn,&QPushButton::customContextMenuRequested,this,&TabWindow::homeContext);
    connect(this->home_btn,&QPushButton::clicked,this,&TabWindow::viewHome);
    hbox->addWidget(this->home_btn);
    this->bookmark_btn->setFlat(true);
    this->bookmark_btn->setIcon(QIcon(":/res/drawables/bookmark.svg"));
    connect(this->bookmark_btn,&QPushButton::clicked,this,&TabWindow::bookmarkPage);
    hbox->addWidget(this->bookmark_btn);
    this->menu_btn->setFlat(true);
    this->menu_btn->setIcon(QIcon(":/res/drawables/menu.svg"));
    hbox->addWidget(menu_btn);
    vbox->addLayout(hbox);
    vbox->addWidget(view);
    vbox->addWidget(pbar);
    pbar->setMaximum(0);
    pbar->setMaximum(100);
    pbar->hide();
    pbar->setMaximumHeight(5);
    pbar->setTextVisible(false);
    tab->setLayout(vbox);
    connect(view,&QWebEngineView::loadStarted,this,&TabWindow::loadBegin);
    connect(view,&QWebEngineView::loadFinished,this,&TabWindow::loadCompleted);
    connect(view,&QWebEngineView::loadProgress,this,&TabWindow::pageProgress);
    tab->setStyleSheet("QWidget{background-color:white} QLineEdit{border:0.5px solid black;border-radius:10px;}");
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
    if(text.startsWith("javascript:")){
        QString script=text.split(":")[1];
        this->view->returnView()->page()->runJavaScript(script);
    }
    else if(textList.length()==1){
        if(text.startsWith("crusta://")||text.startsWith("file://")){
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
    else if(text.startsWith("http://")||text.startsWith("https://")){
        this->view->returnView()->load(QUrl(text));
    }
    else{
        QString searchStr=this->addr_bar->defaultSearch+text;
        this->view->returnView()->load(QUrl(searchStr));
    }
}

void TabWindow::bookmarkPage(){
    QFile file(QDir::homePath()+"/.crusta_db/bookmarks.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out << this->view->returnView()->title().toLatin1()+">>>>>"+this->view->returnView()->url().toString().toLatin1()+">>>>>"+"\n";
    file.close();
    this->bookmark_btn->setIcon(QIcon(":/res/drawables/star.svg"));
}

void TabWindow::updateStar(){
    QString s=this->addr_bar->text();
    QFile input(QDir::homePath()+"/.crusta_db/bookmarks.txt");
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

void TabWindow::homeContext(const QPoint& pos){
    QMenu* menu=new QMenu();
    QAction* go_home=new QAction(tr("Home"));
    QAction* set_home=new QAction(tr("Set Home Page"));
    connect(go_home,&QAction::triggered,this->view->returnView(),&WebView::home);
    connect(set_home,&QAction::triggered,this,&TabWindow::setHomePage);
    menu->addAction(go_home);
    menu->addAction(set_home);
    menu->exec(this->home_btn->mapToGlobal(pos));
}

void TabWindow::setHomePage(){
    QDialog* w=new QDialog();
    QLabel* lbl=new QLabel(tr("Home Page URL"));
    QLineEdit* url=new QLineEdit();
    url->setText(this->view->returnView()->home_page);
    connect(url,&QLineEdit::returnPressed,w,&QDialog::accept);

    QCompleter* c=new QCompleter();
    QStringListModel* m=new QStringListModel();
    QStringList l;
    l.append("https://google.com");
    l.append("https://duckduckgo.com");
    l.append("https://bing.com");
    l.append("https://qwant.com");
    l.append("https://www.yandex.com");
    l.append("https://www.ecosia.org");
    l.append("https://www.baidu.com");
    m->setStringList(l);
    c->setModel(m);
    c->setFilterMode(Qt::MatchContains);
    url->setCompleter(c);

    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(lbl);
    hbox->addWidget(url);
    QHBoxLayout* h1box=new QHBoxLayout();
    QPushButton* cncl=new QPushButton(tr("Cancel"));
    QPushButton* ok=new QPushButton(tr("Save"));
    h1box->addWidget(new QLabel());
    h1box->addWidget(cncl);
    h1box->addWidget(ok);
    cncl->setFixedWidth(100);
    ok->setFixedWidth(100);
    QVBoxLayout* vbox=new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addLayout(h1box);
    w->setLayout(vbox);
    w->setFixedWidth(500);
    w->setWindowTitle("Crusta : Set Home Page");
    w->setStyleSheet("QWidget{background-color:white;color:black} QLabel{color:black} QLineEdit{color:black;background-color:white;border:1px solid black} QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
    connect(cncl,&QPushButton::clicked,w,&QDialog::reject);
    connect(ok,&QPushButton::clicked,w,&QDialog::accept);
    if(w->exec()!=QDialog::Accepted){
        return;
    }
    if(url->text()=="")
        return;
    QString new_string=url->text();
    QFile f(QDir::homePath()+"/.crusta_db/preference.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            QStringList data=line.split(">>>>>");
            if(data[0]=="Home Page")
                s.append(data[0]+">>>>>"+new_string + "\n");
            else
                s.append(line+"\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
    this->view->returnView()->home_page=new_string;
}

void TabWindow::pageProgress(int p){
    pbar->setValue(p);
}

void TabWindow::loadBegin(){
    pbar->show();
    this->load_btn->setIcon(QIcon(":/res/drawables/close.svg"));
    load_btn->disconnect();
    connect(load_btn,&QPushButton::clicked,view,&QWebEngineView::stop);
}

void TabWindow::loadCompleted(){
    pbar->hide();
    load_btn->disconnect();
    this->load_btn->setIcon(QIcon(":/res/drawables/reload.svg"));
    connect(load_btn,&QPushButton::clicked,view,&QWebEngineView::reload);
}
