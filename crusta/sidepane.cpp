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
#include <QRect>

#include <iostream>


SidePaneButton::SidePaneButton(){
    setStyleSheet("QPushButton{border: none;margin: 0} QPushButton::hover{background-color: #000;}");
    setFixedSize(48,48);
}

SidePane::SidePane(MainView* m){
    mainview=m;
    QHBoxLayout* hbox=new QHBoxLayout();
    QWidget* left=new QWidget();
    QVBoxLayout* vbox=new QVBoxLayout();
    vbox->addWidget(history);
    history->setToolTip(tr("History"));
    history->setIcon(QIcon(":/res/drawables/pane_history.svg"));
    vbox->addWidget(bookmarks);
    bookmarks->setToolTip(tr("Bookmarks"));
    bookmarks->setIcon(QIcon(":/res/drawables/pane_bookmark.svg"));
    vbox->addWidget(downloads);
    downloads->setToolTip(tr("Downloads"));
    downloads->setIcon(QIcon(":/res/drawables/pane_download.svg"));
    QLabel* flexilabel=new QLabel();
    vbox->addWidget(flexilabel);
    vbox->addWidget(add_pane_btn);
    add_pane_btn->setToolTip(tr("Add New Pane Button"));
    add_pane_btn->setIcon(QIcon(":/res/drawables/pane_add.svg"));
    vbox->setSpacing(0);
    vbox->setContentsMargins(0,0,0,0);
    left->setLayout(vbox);
    left->setStyleSheet("background-color: #404244");
    left->setFixedWidth(48);
    left->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Expanding);
    hbox->addWidget(left);
    connect(history,&QPushButton::clicked,this,[this,hbox]{
        if(hbox->count()==1){
            history_manager=new HistoryManager(mainview);
            history_manager->createManager();
            hbox->addWidget(this->history_manager);
        }
        else if(hbox->indexOf(this->history_manager)!=1){
            hbox->itemAt(1)->widget()->hide();
            hbox->removeItem(hbox->itemAt(1));
            history_manager=new HistoryManager(mainview);
            history_manager->createManager();
            hbox->addWidget(this->history_manager);
        }
        else{
            this->history_manager->hide();
            hbox->removeWidget(this->history_manager);
        }
    });
    connect(bookmarks,&QPushButton::clicked,this,[this,hbox]{
        if(hbox->count()==1){
            bookmark_manager=new BookmarkManager(mainview);
            hbox->addWidget(this->bookmark_manager);
        }
        else if(hbox->indexOf(this->bookmark_manager)!=1){
            hbox->itemAt(1)->widget()->hide();
            hbox->removeItem(hbox->itemAt(1));
            bookmark_manager=new BookmarkManager(mainview);
            hbox->addWidget(this->bookmark_manager);
        }
        else{
            this->bookmark_manager->hide();
            hbox->removeWidget(this->bookmark_manager);
        }
    });
    connect(downloads,&QPushButton::clicked,this,[this,hbox]{
        if(hbox->count()==1){
            this->download_manager->show();
            hbox->addWidget(this->download_manager);
        }
        else if(hbox->indexOf(this->download_manager)!=1){
            hbox->itemAt(1)->widget()->hide();
            hbox->removeItem(hbox->itemAt(1));
            this->download_manager->show();
            hbox->addWidget(this->download_manager);
        }
        else{
            this->download_manager->hide();
            hbox->removeWidget(this->download_manager);
        }
    });
    connect(add_pane_btn,&SidePaneButton::clicked,this,&SidePane::addNewButton);
    hbox->setSpacing(0);
    hbox->setContentsMargins(0,0,0,0);
    setLayout(hbox);
    setMaximumWidth(48+350+45);
    setObjectName("pane");
    setStyleSheet("#pane{background-color: #404244}");
}

void SidePane::addNewButton(){
    QDialog* dg=new QDialog(mainview);
    QLineEdit* urledit=new QLineEdit();
    urledit->setMinimumWidth(200);
    urledit->setPlaceholderText("url for side panel");
    QPushButton* ok=new QPushButton(tr("Add"));
    QPushButton* cncl=new QPushButton(tr("Cancel"));
    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(urledit);
    hbox->addWidget(ok);
    hbox->addWidget(cncl);
    dg->setLayout(hbox);
    dg->setWindowFlag(Qt::FramelessWindowHint);
    connect(cncl,&QPushButton::clicked,dg,&QDialog::reject);
    connect(ok,&QPushButton::clicked,dg,&QDialog::accept);
    dg->move(mapToGlobal(QPoint(add_pane_btn->x()+30,add_pane_btn->y()-30)));
    dg->setObjectName("dialog");
    dg->setStyleSheet("#dialog{border: 2px solid #404244}");
    if(!dg->exec()==QDialog::Accepted){
        return;
    }
    //save the panel
}
