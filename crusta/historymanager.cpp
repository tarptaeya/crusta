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

#include "historymanager.h"
#include "webview.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTreeWidget>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QTreeWidgetItem>
#include <QDir>
#include <QIcon>
#include <QStringList>
#include <QString>
#include <QMenu>
#include <QIODevice>

#include <iostream>




void HistoryManager::createManager(){
    setWindowTitle(tr("Crusta : History Manager"));
    setStyleSheet("QWidget{background-color:blueviolet;color:white} QTreeWidget{background-color:white;color:blueviolet} QPushButton{border:0.5px solid crimson;padding:4px 8px;color:white;background-color:crimson} QPushButton:hover{background-color:white;color:crimson}");
    QFile inputFile("history.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(!(data[0]=="" || data[1]=="" || data[2]=="")){
              QTreeWidgetItem* item=new QTreeWidgetItem(data);
              display_area->insertTopLevelItem(0,item);
          }
       }
       inputFile.close();
    }
}

HistoryManager::HistoryManager(MainView *m){
    mview=m;
    vbox->addWidget(display_area);
    QHBoxLayout* h1box=new QHBoxLayout();
    h1box->addWidget(clear_all);
    clear_all->setFixedWidth(125);
    h1box->addWidget(new QLabel());
    vbox->addLayout(h1box);
    setLayout(vbox);
    QStringList header;
    header.append(tr("Title"));
    header.append(tr("URL"));
    header.append(tr("Date"));
    display_area->setColumnCount(header.count());
    display_area->setHeaderLabels(header);
    display_area->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(display_area,&QTreeWidget::customContextMenuRequested,this,&HistoryManager::showContextMenu);
    connect(clear_all,&QPushButton::clicked,this,&HistoryManager::clearAll);
    connect(open,&QAction::triggered,this,&HistoryManager::openUrl);
    connect(del,&QAction::triggered,this,&HistoryManager::clearEntry);
}

void HistoryManager::showContextMenu(const QPoint &pos){
    if(display_area->itemAt(pos)==NULL)return;
    QMenu* cmenu=new QMenu();
    cmenu->addAction(open);
    cmenu->addAction(del);
    cmenu->setStyleSheet("QMenu{background-color:white;color:blueviolet} QMenu::selected{color:white;background-color:blueviolet}");
    cmenu->exec(display_area->mapToGlobal(pos));
}

void HistoryManager::clearAll(){
    QFile file("history.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out <<"";
    file.close();
    display_area->clear();
}

void HistoryManager::openUrl(){
    QTreeWidgetItem* item=display_area->currentItem();
    QUrl url=QUrl(item->text(1));
    mview->addNormalTab();
    int index=mview->tabWindow->count()-1;
    mview->tabWindow->setCurrentIndex(index);
    QWidget* widget=mview->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    WebView* webview=(WebView*)layout->itemAt(1)->widget();
    webview->load(url);
}

void HistoryManager::clearEntry(){
    QTreeWidgetItem* item=display_area->currentItem();
    QString forbidden=item->text(0).toLatin1()+">>>>>"+item->text(1).toLatin1()+">>>>>"+item->text(2).toLatin1();
    QFile f("history.txt");
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
