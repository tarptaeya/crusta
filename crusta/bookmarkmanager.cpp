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

BookmarkManager::BookmarkManager(MainView *m){
    mview=m;
    setLayout(vbox);
    setStyleSheet("QWidget{background-color:blueviolet;color:white} QTreeWidget{background-color:white;color:blueviolet} QPushButton{border:0.5px solid crimson;padding:4px 8px;color:white;background-color:crimson} QPushButton:hover{background-color:white;color:crimson}");
    vbox->addWidget(display);
    QStringList header;
    header.append("Title");
    header.append("URL");
    header.append("Description");
    display->setHeaderLabels(header);
    setWindowTitle("Crusta : Bookmarks Manager");
    loadBookmarks();
    display->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(display,&QTreeWidget::customContextMenuRequested,this,&BookmarkManager::showContextMenu);
    connect(open,&QAction::triggered,this,&BookmarkManager::openUrl);
    connect(del,&QAction::triggered,this,&BookmarkManager::clearEntry);
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
    cmenu->setStyleSheet("QMenu{background-color:white;color:blueviolet} QMenu::selected{color:white;background-color:blueviolet}");
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

