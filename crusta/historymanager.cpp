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

#include <iostream>




void HistoryManager::createManager(){
    createHeader();
    retrieveHistory();
    setLayout(vbox);
    lbl->setText("Automatically delete history after ");
    time->addItem("1 Week");
    time->addItem("1 Month");
    time->addItem("2 Months");
    time->addItem("3 Months");
    searchline->setPlaceholderText("Search ...");
    searchline->setFixedWidth(150);
    hbox->addWidget(lbl);
    hbox->addWidget(time);
    hbox->addWidget(new QLabel());
    hbox->addWidget(searchline);
    h1box->addWidget(treeview);
    del_btn->setText("Remove");
    del_btn->setFixedWidth(80);
    clear_all->setText("Clear All");
    clear_all->setFixedWidth(80);
    h2box->addWidget(del_btn);
    h2box->addWidget(clear_all);
    h2box->addWidget(new QLabel());
    vbox->addLayout(hbox);
    vbox->addLayout(h1box);
    vbox->addLayout(h2box);
}

void HistoryManager::createHeader(){
    QStringList labels;
    labels.push_back("title");
    labels.push_back("address");
    labels.push_back("date & time");
    treeview->setColumnCount(3);
    treeview->setHeaderLabels(labels);
}

void HistoryManager::retrieveHistory(){
    QString filename=QDir::currentPath()+QString("/history/hist_data.txt");
    QFile file(filename);
    if ( !file.open(QIODevice::ReadOnly|QIODevice::Text) )return;
    else{
        QTextStream in(&file);
        while(!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split("<<<");
            QTreeWidgetItem* it_items=new QTreeWidgetItem();
            it_items->setText(0,fields[0]);
            QString name=fields[1].split("/")[2];
            QStringList names=name.split(".");
            if(names[0]==QString("www")){
                name=names[1];
            }
            else{
                name=names[0];
            }
            QString icon_name=QDir::currentPath()+QString("/history/favicons/")+name+QString(".png");
            it_items->setIcon(0,QIcon(icon_name));
            it_items->setText(1,fields[1]);
            it_items->setText(2,fields[2]);
            treeview->insertTopLevelItem(0,it_items);
        }
    }
    file.close();
}
