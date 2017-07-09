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

#include "sidepane.h"
#include <QLabel>

#include <iostream>


SidePaneButton::SidePaneButton(){
    setStyleSheet("QPushButton{border: none;margin: 0}");
    setFixedSize(27,27);
}

SidePane::SidePane(MainView* m){
    mainview=m;
    QHBoxLayout* hbox=new QHBoxLayout();
    QWidget* left=new QWidget();
    QVBoxLayout* vbox=new QVBoxLayout();
    vbox->addWidget(history);
    history->setIcon(QIcon(":/res/drawables/pane_history.svg"));
    vbox->addWidget(bookmarks);
    bookmarks->setIcon(QIcon(":/res/drawables/pane_bookmark.svg"));
    vbox->addWidget(downloads);
    downloads->setIcon(QIcon(":/res/drawables/pane_download.svg"));
    QLabel* flexilabel=new QLabel();
    vbox->addWidget(flexilabel);
    vbox->addWidget(add_pane_btn);
    add_pane_btn->setIcon(QIcon(":/res/drawables/pane_add.svg"));
    vbox->setSpacing(20);
    left->setLayout(vbox);
    left->setStyleSheet("background-color: #404244");
    left->setFixedWidth(48);
    left->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    hbox->addWidget(left);
    connect(history,&QPushButton::clicked,this,[this,hbox]{
        std::cout<<hbox->count()<<std::endl;
        std::cout<<hbox->indexOf(this->history_manager)<<std::endl;
        if(hbox->count()==1){
            history_manager=new HistoryManager(mainview);
            history_manager->createManager();
            hbox->addWidget(this->history_manager);
        }
        else if(hbox->indexOf(this->history_manager)!=1){
            hbox->removeItem(hbox->itemAt(1));
            history_manager=new HistoryManager(mainview);
            history_manager->createManager();
            hbox->addWidget(this->history_manager);
        }
        else{
            hbox->removeWidget(this->history_manager);
        }
    });
    connect(downloads,&QPushButton::clicked,this,[this,hbox]{
        std::cout<<hbox->count()<<std::endl;
        std::cout<<hbox->indexOf(this->download_manager)<<std::endl;
        if(hbox->count()==1){
            hbox->addWidget(this->download_manager);
        }
        else if(hbox->indexOf(this->download_manager)!=1){
            hbox->removeItem(hbox->itemAt(1));
            hbox->addWidget(this->download_manager);
        }
        else{
            hbox->removeWidget(this->download_manager);
        }
    });
    hbox->setSpacing(0);
    hbox->setContentsMargins(0,0,0,0);
    setLayout(hbox);
    setMaximumWidth(48+350+45);
    setObjectName("pane");
    setStyleSheet("#pane{background-color: #404244}");
}
