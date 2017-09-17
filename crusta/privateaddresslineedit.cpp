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

#include "privateaddresslineedit.h"
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>
#include <QMenu>
#include <QString>
#include <QStringList>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QCompleter>
#include <QStringListModel>
#include <QDir>
#include <QWebEngineProfile>

void PrivateAddressLineEdit::createAddressLineEdit(){
    siteinfo_btn->move(3,3);
    siteinfo_btn->setFixedSize(20,20);
    siteinfo_btn->setIcon(QIcon(":/res/drawables/normal_site.svg"));
    siteinfo_btn->setStyleSheet("border: 1px solid #00b0e3");
    siteinfo_btn->setToolTip(tr("Site Info"));
    setTextMargins(22,0,7,0);
    setFixedHeight(26);
    loadSearchString();
}

QLineEdit* PrivateAddressLineEdit::initialize(){
    return this;
}

void PrivateAddressLineEdit::showContextMenu(const QPoint& pos){
    QMenu* contextMenu=new QMenu();
    QAction* undo=new QAction(tr("Undo"));
    connect(undo,&QAction::triggered,this,&QLineEdit::undo);
    contextMenu->addAction(undo);
    QAction* redo=new QAction(tr("Redo"));
    connect(redo,&QAction::triggered,this,&QLineEdit::redo);
    contextMenu->addAction(redo);
    contextMenu->addSeparator();
    QAction* cut=new QAction(tr("Cut"));
    connect(cut,&QAction::triggered,this,&QLineEdit::cut);
    contextMenu->addAction(cut);
    QAction* copy=new QAction(tr("Copy"));
    connect(copy,&QAction::triggered,this,&QLineEdit::copy);
    contextMenu->addAction(copy);
    QAction* paste=new QAction(tr("Paste"));
    connect(paste,&QAction::triggered,this,&QLineEdit::paste);
    contextMenu->addAction(paste);
    contextMenu->addSeparator();
    contextMenu->addAction(default_search);
    contextMenu->addAction(changeUAstring);
    contextMenu->exec(this->mapToGlobal(pos));
}

PrivateAddressLineEdit::PrivateAddressLineEdit(){
    setPlaceholderText(tr("Search or enter address"));
    createAddressLineEdit();
    createCompleter();
    setCompleter(cmpleter);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&PrivateAddressLineEdit::customContextMenuRequested,this,&PrivateAddressLineEdit::showContextMenu);
    connect(default_search,&QAction::triggered,this,&PrivateAddressLineEdit::setDefaultSearch);
    connect(changeUAstring,&QAction::triggered,this,&PrivateAddressLineEdit::setUAString);
}

void PrivateAddressLineEdit::createCompleter(){
    QStringListModel* model=new QStringListModel();
    cmpleter->setModel(model);
    cmpleter->setCaseSensitivity(Qt::CaseInsensitive);
    cmpleter->setFilterMode(Qt::MatchContains);
    QFile inputFile(QDir::homePath()+"/.crusta_db/completer.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          list.append(line);
       }
       inputFile.close();
    }
    model->setStringList(list);
}

void PrivateAddressLineEdit::loadSearchString(){
    QFile inputFile(QDir::homePath()+"/.crusta_db/preference.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data[0]=="Incognito Search String"){
              this->defaultSearch=data[1];
              inputFile.close();
              if(this->defaultSearch.isEmpty())
                  this->defaultSearch=QString("https://duckduckgo.com/?q=");
              return;
          }
       }
       inputFile.close();
    }
}

void PrivateAddressLineEdit::setDefaultSearch(){
    QDialog* w=new QDialog();
    QLabel* lbl=new QLabel(tr("Search String"));
    QLineEdit* url=new QLineEdit();
    url->setText(defaultSearch);

    QCompleter* c=new QCompleter();
    QStringListModel* m=new QStringListModel();
    QStringList l;
    l.append("https://www.ecosia.org/search?tt=crusta&q=");
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
    w->setWindowTitle("Crusta : Set Default Search Engine");
    //w->setStyleSheet("QWidget{background-color:white;color:black} QLabel{color:black} QLineEdit{color:black;background-color:white;border: 1px solid black} QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
    connect(cncl,&QPushButton::clicked,w,&QDialog::reject);
    connect(ok,&QPushButton::clicked,w,&QDialog::accept);
    if(w->exec()!=QDialog::Accepted){
        return;
    }
    if(url->text()=="")
        return;
    QString new_string=url->text();
    defaultSearch=new_string;
    QFile f(QDir::homePath()+"/.crusta_db/preference.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            QStringList data=line.split(">>>>>");
            if(data[0]=="Incognito Search String")
                s.append(data[0]+">>>>>"+new_string + "\n");
            else
                s.append(line+"\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
}

void PrivateAddressLineEdit::setUAString(){
    QDialog* w=new QDialog();
    QLabel* lbl=new QLabel(tr("HTTP USER AGENT"));
    QLineEdit* ua=new QLineEdit();
    QString http;
    QFile inputFile(QDir::homePath()+"/.crusta_db/preference.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data[0]=="Incognito UA String"){
              http=data[1];
              break;
          }
       }
       inputFile.close();
    }
    ua->setText(http);
    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(lbl);
    hbox->addWidget(ua);
    QHBoxLayout* h1box=new QHBoxLayout();
    QPushButton* restore=new QPushButton(tr("Restore"));
    QPushButton* cncl=new QPushButton(tr("Cancel"));
    QPushButton* ok=new QPushButton(tr("Save"));
    h1box->addWidget(restore);
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
    w->setWindowTitle("Crusta : Edit HTTP User Agent String");
    //w->setStyleSheet("QWidget{background-color:white;color:black} QLabel{color:black} QLineEdit{color:black;background-color:white;border:1px solid black} QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
    connect(cncl,&QPushButton::clicked,w,&QDialog::reject);
    connect(ok,&QPushButton::clicked,w,&QDialog::accept);
    connect(restore,&QPushButton::clicked,this,[this,w]{restoreUAString();w->reject();});
    if(w->exec()!=QDialog::Accepted){
        return;
    }
    if(ua->text()=="")
        return;
    QString new_string=ua->text();
    QFile f(QDir::homePath()+"/.crusta_db/preference.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            QStringList data=line.split(">>>>>");
            if(data[0]=="Incognito UA String")
                s.append(data[0]+">>>>>"+new_string + "\n");
            else
                s.append(line+"\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
}

void PrivateAddressLineEdit::restoreUAString(){
    QWebEngineProfile p;
    p.setHttpUserAgent("");
    QStringList ua=p.httpUserAgent().split(" ");
    QString new_string="";
    int len=ua.length();
    for(int i=0;i<len-1;i++){
        new_string+=ua[i]+" ";
    }
    new_string+="Crusta/1.0.0 "+ua[len-1];
    QFile f(QDir::homePath()+"/.crusta_db/preference.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            QStringList data=line.split(">>>>>");
            if(data[0]=="Incognito UA String")
                s.append(data[0]+">>>>>"+new_string + "\n");
            else
                s.append(line+"\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
}
