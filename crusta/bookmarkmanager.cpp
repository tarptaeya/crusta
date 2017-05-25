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

#include "bookmarkmanager.h"

#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QStringList>
#include <QString>

#include <iostream>

BookmarkManager::BookmarkManager(MainView *m){
    mview=m;
    setLayout(vbox);
    setStyleSheet("QWidget{background-color:white;color:black} QLineEdit{background-color:white;color:black;border:1px solid black} QTreeWidget{background-color:white;color:black} QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel());
    hbox->addWidget(search);
    hbox->addWidget(sbtn);
    search->setPlaceholderText(tr("Search..."));
    search->setFixedWidth(200);
    sbtn->setFixedWidth(50);
    vbox->addLayout(hbox);
    vbox->addWidget(display);
    QStringList header;
    header.append(tr("Title"));
    header.append(tr("URL"));
    header.append(tr("Description"));
    display->setHeaderLabels(header);
    setWindowTitle(tr("Crusta : Bookmarks Manager"));
    loadBookmarks();
    display->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(display,&QTreeWidget::customContextMenuRequested,this,&BookmarkManager::showContextMenu);
    connect(open,&QAction::triggered,this,&BookmarkManager::openUrl);
    connect(del,&QAction::triggered,this,&BookmarkManager::clearEntry);
    QHBoxLayout* h1box=new QHBoxLayout();
    h1box->addWidget(new QLabel(tr("Edit Desription : ")));
    h1box->addWidget(description);
    h1box->addWidget(save);
    vbox->addLayout(h1box);
    connect(save,&QPushButton::clicked,this,&BookmarkManager::saveDescription);
    connect(sbtn,&QPushButton::clicked,this,&BookmarkManager::searchBookmark);
}

void BookmarkManager::loadBookmarks(){
    QFile inputFile("bookmarks.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data.count()==1)continue;
          if(data.count()==2)data.append("");
          if(!(data[0]=="" || data[1]=="")){
              QTreeWidgetItem* item=new QTreeWidgetItem(data);
              display->insertTopLevelItem(0,item);

          }
       }
       inputFile.close();
    }
}

void BookmarkManager::showContextMenu(const QPoint &pos){
    if(display->itemAt(pos)==NULL)return;
    QMenu* cmenu=new QMenu();
    cmenu->addAction(open);
    cmenu->addAction(del);
    cmenu->setStyleSheet("QMenu{background-color:white;color:black} QMenu::selected{color:white;background-color:black}");
    cmenu->exec(display->mapToGlobal(pos));
}

void BookmarkManager::openUrl(){
    QTreeWidgetItem* item=display->currentItem();
    QUrl url=QUrl(item->text(1));
    mview->addNormalTab();
    int index=mview->tabWindow->count()-1;
    mview->tabWindow->setCurrentIndex(index);
    QWidget* widget=mview->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    WebView* webview=(WebView*)layout->itemAt(1)->widget();
    webview->load(url);
}

void BookmarkManager::clearEntry(){
    QTreeWidgetItem* item=display->currentItem();
    QString forbidden=item->text(0).toLatin1()+">>>>>"+item->text(1).toLatin1()+">>>>>"+item->text(2).toLatin1();
    QFile f("bookmarks.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(line!=forbidden)
                s.append(line + "\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
    delete item;
}

void BookmarkManager::saveDescription(){
    if(display->currentItem()==NULL)
        return;
    QTreeWidgetItem* item=display->currentItem();
    QString forbidden=item->text(0).toLatin1()+">>>>>"+item->text(1).toLatin1()+">>>>>"+item->text(2).toLatin1();
    item->setText(2,description->text());
    description->setText("");
    QString newline=item->text(0).toLatin1()+">>>>>"+item->text(1).toLatin1()+">>>>>"+item->text(2).toLatin1();
    QFile f("bookmarks.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(line!=forbidden)
                s.append(line + "\n");
            else
                s.append(newline+"\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
}

void BookmarkManager::searchBookmark(){
    QString key=search->text();
    display->clear();
    QFile inputFile("bookmarks.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data.count()==1)continue;
          if(data.count()==2)data.append("");
          QString master=data[2];
          if(master.contains(key)){
              QTreeWidgetItem* item=new QTreeWidgetItem(data);
              display->insertTopLevelItem(0,item);

          }
       }
       inputFile.close();
    }
}

