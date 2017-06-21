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
#include <QDate>

#include <iostream>




void HistoryManager::createManager(){
    setWindowTitle(tr("Crusta : History Manager"));
    setStyleSheet("QWidget{background-color:white;color:black} QComboBox{background-color:white;color:black} QComboBox QAbstractItemView{background-color:white;color:black} QTreeWidget{background-color:white;color:black} QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
    QFile inputFile(QDir::homePath()+"/.crusta_db/history.txt");
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
    QHBoxLayout* h2box=new QHBoxLayout();
    h2box->addWidget(new QLabel());
    h2box->addWidget(date);
    date->addItem(tr("All Time"));
    date->addItem(tr("Today"));
    date->addItem(tr("Yesterday"));
    date->addItem(tr("This Week"));
    date->addItem(tr("This Month"));
    date->addItem(tr("Last 3 Months"));
    connect(date,&QComboBox::currentTextChanged,this,&HistoryManager::setFilterDate);
    vbox->addLayout(h2box);
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
    cmenu->setStyleSheet("QMenu{background-color:white;color:black} QMenu::selected{color:white;background-color:black}");
    cmenu->exec(display_area->mapToGlobal(pos));
}

void HistoryManager::clearAll(){
    QFile file(QDir::homePath()+"/.crusta_db/history.txt");
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
    QFile f(QDir::homePath()+"/.crusta_db/history.txt");
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

void HistoryManager::setFilterDate(){
    QDate filterDate=QDate::currentDate();
    switch (date->currentIndex()) {
    case 1:
        filterDate=filterDate.addDays(0);
        break;
    case 2:
        filterDate=filterDate.addDays(-1);
        break;
    case 3:
        filterDate=filterDate.addDays(-7);
        break;
    case 4:
        filterDate=filterDate.addMonths(-1);
        break;
    case 5:
        filterDate=filterDate.addMonths(-3);
        break;
    case 0:
        display_area->clear();
        QFile inputFile(QDir::homePath()+"/.crusta_db/history.txt");
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
        return;
    }
    display_area->clear();
    int index=date->currentIndex();
    QFile inputFile(QDir::homePath()+"/.crusta_db/history.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(!(data[0]=="" || data[1]=="" || data[2]=="")){
              QDate itemDate=QDate::fromString(data[2]);
              if(itemDate>=filterDate && index!=2){
                    QTreeWidgetItem* item=new QTreeWidgetItem(data);
                    display_area->insertTopLevelItem(0,item);
              }
              if(itemDate==filterDate && index==2){
                    QTreeWidgetItem* item=new QTreeWidgetItem(data);
                    display_area->insertTopLevelItem(0,item);
              }
          }
       }
       inputFile.close();
    }
}
