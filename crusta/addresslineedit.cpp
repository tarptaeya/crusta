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

#include "addresslineedit.h"
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>
#include <QMenu>

#include <iostream>


void AddressLineEdit::createAddressLineEdit(){
    this->setTextMargins(15,0,15,0);
//    this->info_btn->setIcon(QIcon(":/res/drawables/info.png"));
//    this->info_btn->setFlat(true);
//    this->info_btn->setParent(this->addr_bar);
//    this->secure_btn->setIcon(QIcon(":/res/drawables/secure.png"));
//    this->secure_btn->setFlat(true);
//    this->secure_btn->setParent(this->addr_bar);
//    this->secure_btn->move(20,0);
}

QLineEdit* AddressLineEdit::initialize(){
    return this;
}

void AddressLineEdit::showContextMenu(const QPoint& pos){
    QMenu* contextMenu=new QMenu();
    QAction* undo=new QAction("Undo");
    contextMenu->addAction(undo);
    QAction* redo=new QAction("Redo");
    contextMenu->addAction(redo);
    contextMenu->addSeparator();
    QAction* cut=new QAction("Cut");
    contextMenu->addAction(cut);
    QAction* copy=new QAction("Copy");
    contextMenu->addAction(copy);
    QAction* paste=new QAction("Paste");
    contextMenu->addAction(paste);
    contextMenu->addSeparator();
    QAction* default_search=new QAction("Search Preference");
    contextMenu->addAction(default_search);
    contextMenu->setStyleSheet("QMenu{background-color:white;color:blueviolet} QMenu::selected{background-color:blueviolet;color:white}");
    contextMenu->exec(this->mapToGlobal(pos));
}

AddressLineEdit::AddressLineEdit(){
    createAddressLineEdit();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&AddressLineEdit::customContextMenuRequested,this,&AddressLineEdit::showContextMenu);
}







