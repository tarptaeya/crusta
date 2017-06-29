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

#include "privatetabwindow.h"
#include "privatewebview.h"

#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QFile>
#include <QMenu>
#include <QTextStream>
#include <QIODevice>
#include <QString>
#include <QWebEngineProfile>
#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QCompleter>
#include <QDir>



void PrivateTabWindow::viewHome(){
    this->view->returnPrivateView()->load(QUrl(this->view->home_page));
}

void PrivateTabWindow::updateAddrBar(){
    QString url=this->view->returnPrivateView()->url().toString();
    QDir* exec_dir=new QDir(QDir::homePath()+"/.crusta_db");
    exec_dir->cd("web");
    QString forbidden;
    if(exec_dir->absolutePath().startsWith("/"))
        forbidden="file://"+exec_dir->absolutePath()+"/index.html";
    else
        forbidden="file:///"+exec_dir->absolutePath()+"/index.html";
    if(forbidden==url){
        return;
    }
    QDir* viewer_file=new QDir(QCoreApplication::applicationDirPath());
    viewer_file->cd("3rd_party/pdfjs/web");
    QString forbidden_;
    if(viewer_file->absolutePath().startsWith("/"))
        forbidden_="file://"+viewer_file->absolutePath()+"/viewer.html?file";
    else
        forbidden_="file:///"+viewer_file->absolutePath()+"/viewer.html?file";
    if(forbidden_==url.split("=")[0]){
        this->addr_bar->initialize()->setText(url.split("/")[url.split("/").length()-1]);
        this->addr_bar->initialize()->setCursorPosition(0);
        return;
    }
    this->addr_bar->initialize()->setText(url);
    this->addr_bar->initialize()->setCursorPosition(0);
}

void PrivateTabWindow::createControls(){
    QHBoxLayout* hbox=new QHBoxLayout();
    this->back_btn->setFlat(true);
    this->back_btn->setIcon(QIcon(":/res/drawables/back.svg"));
    connect(this->back_btn,&QPushButton::clicked,this->view->returnPrivateView(),&QWebEngineView::back);
    hbox->addWidget(this->back_btn);
    this->fwd_btn->setFlat(true);
    this->fwd_btn->setIcon(QIcon(":/res/drawables/forward.svg"));
    connect(this->fwd_btn,&QPushButton::clicked,this->view->returnPrivateView(),&QWebEngineView::forward);
    hbox->addWidget(this->fwd_btn);
    this->load_btn->setFlat(true);
    this->load_btn->setIcon(QIcon(":/res/drawables/reload.svg"));
    connect(this->load_btn,&QPushButton::clicked,this->view->returnPrivateView(),&QWebEngineView::reload);
    hbox->addWidget(this->load_btn);
    hbox->addWidget(this->addr_bar->initialize());
    connect(this->view->returnPrivateView(),&QWebEngineView::urlChanged,this,&PrivateTabWindow::updateAddrBar);
    connect(this->view->returnPrivateView(),&QWebEngineView::loadFinished,this,&PrivateTabWindow::updateStar);
    connect(this->addr_bar->initialize(),&QLineEdit::returnPressed,this,&PrivateTabWindow::loadUrl);
    //hbox->addWidget(this->search_bar->initialize());
    this->home_btn->setFlat(true);
    this->home_btn->setIcon(QIcon(":/res/drawables/home.svg"));
    this->home_btn->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->home_btn,&QPushButton::customContextMenuRequested,this,&PrivateTabWindow::homeContext);
    connect(this->home_btn,&QPushButton::clicked,this,&PrivateTabWindow::viewHome);
    hbox->addWidget(this->home_btn);
    this->bookmark_btn->setFlat(true);
    this->bookmark_btn->setIcon(QIcon(":/res/drawables/bookmark.svg"));
    connect(this->bookmark_btn,&QPushButton::clicked,this,&PrivateTabWindow::bookmarkPage);
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
    connect(view,&QWebEngineView::loadStarted,this,&PrivateTabWindow::loadBegin);
    connect(view,&QWebEngineView::loadFinished,this,&PrivateTabWindow::loadCompleted);
    connect(view,&QWebEngineView::loadProgress,this,&PrivateTabWindow::pageProgress);
    tab->setLayout(vbox);
    vbox->setSpacing(0);
    tab->setStyleSheet("QWidget{background-color:white} QLineEdit{border:0.5px solid black;border-radius:10px;background-color:white;color:black} QPushButton{border: none;margin-left: 10px;} QPushButton::hover{background-color: #f2f2f2}");
}

QWidget* PrivateTabWindow::returnTab(){
    this->vbox->setContentsMargins(0,0,0,0);
    createControls();
    return this->tab;
}

PrivateTabWindow* PrivateTabWindow::returnThis(){
    this->vbox->setContentsMargins(0,0,0,0);
    createControls();
    return this;
}

void PrivateTabWindow::setWebView(PrivateWebView* v){
    view=v;
}

QWidget* PrivateTabWindow::returnTab(PrivateWebView* view){
    this->vbox->setContentsMargins(0,0,0,0);
    setWebView(view);
    createControls();
    return this->tab;
}

void PrivateTabWindow::loadUrl(){
    QString text=this->addr_bar->initialize()->text();
    QStringList textList=text.split(" ");
    if(text.startsWith("javascript:")){
        QString script=text.split(":")[1];
        this->view->returnPrivateView()->page()->runJavaScript(script);
    }
    else if(textList.length()==1){
        if(text.startsWith("crusta://")||text.startsWith("file://")){
            this->view->returnPrivateView()->load(QUrl(text));
        }
        else if(text.startsWith("localhost:")||text=="localhost"){
            this->view->returnPrivateView()->load(QUrl("http://"+text));
        }
        else if(text.startsWith("http://localhost")||text.startsWith("http://localhost:")){
            this->view->returnPrivateView()->load(QUrl(text));
        }
        else if(text.split('.').length()==1){
            QString searchStr=this->addr_bar->defaultSearch+text;
            this->view->returnPrivateView()->load(QUrl(searchStr));
        }
        else{
            if(!(text.startsWith("http://")||text.startsWith("https://"))){
                text="http://"+text;
            }
            this->view->returnPrivateView()->load(QUrl(text));
        }
    }
    else if(text.startsWith("http://")||text.startsWith("https://")){
        this->view->returnPrivateView()->load(QUrl(text));
    }
    else{
        QString searchStr=this->addr_bar->defaultSearch+text;
        this->view->returnPrivateView()->load(QUrl(searchStr));
    }
}

void PrivateTabWindow::bookmarkPage(){
    QFile file(QDir::homePath()+"/.crusta_db/bookmarks.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out << this->view->returnPrivateView()->title().toLatin1()+">>>>>"+this->view->returnPrivateView()->url().toString().toLatin1()+">>>>>"+"\n";
    file.close();
    this->bookmark_btn->setIcon(QIcon(":/res/drawables/star.svg"));
}

void PrivateTabWindow::updateStar(){
    QString s=this->addr_bar->text();
    QFile input(QDir::homePath()+"bookmarks.txt");
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

void PrivateTabWindow::homeContext(const QPoint& pos){
    QMenu* menu=new QMenu();
    QAction* go_home=new QAction(tr("Home"));
    QAction* set_home=new QAction(tr("Set Home Page"));
    connect(go_home,&QAction::triggered,this->view->returnPrivateView(),&PrivateWebView::home);
    connect(set_home,&QAction::triggered,this,&PrivateTabWindow::setHomePage);
    menu->addAction(go_home);
    menu->addAction(set_home);
    menu->exec(this->home_btn->mapToGlobal(pos));
}

void PrivateTabWindow::setHomePage(){
    QDialog* w=new QDialog();
    QLabel* lbl=new QLabel(tr("Home Page URL"));
    QLineEdit* url=new QLineEdit();
    url->setText(this->view->returnPrivateView()->home_page);
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
    //w->setStyleSheet("QWidget{background-color:white;color:black} QLabel{color:black} QLineEdit{color:black;background-color:white;border:1px solid black} QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
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
            if(data[0]=="Incognito Home Page")
                s.append(data[0]+">>>>>"+new_string + "\n");
            else
                s.append(line+"\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
    this->view->returnPrivateView()->home_page=new_string;
}

void PrivateTabWindow::pageProgress(int p){
    pbar->setValue(p);
}

void PrivateTabWindow::loadBegin(){
    pbar->show();
    this->load_btn->setIcon(QIcon(":/res/drawables/close.svg"));
    load_btn->disconnect();
    connect(load_btn,&QPushButton::clicked,view,&QWebEngineView::stop);
}

void PrivateTabWindow::loadCompleted(){
    pbar->hide();
    load_btn->disconnect();
    this->load_btn->setIcon(QIcon(":/res/drawables/reload.svg"));
    connect(load_btn,&QPushButton::clicked,view,&QWebEngineView::reload);
}
