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

#include <searchlineedit.h>
#include <QPushButton>
#include <QMenu>
#include <QLineEdit>
#include <QIcon>

void SearchLineEdit::createSearchLineEdit(){
    this->text->setTextMargins(37,0,0,0);
    this->text->setStyleSheet("max-width:170px");
    createSearchOptions();
}

void SearchLineEdit::createSearchOptions(){
    this->menu->addAction(QIcon(":/res/favicons/google.png"),"&Google");
    this->menu->addAction(QIcon(":/res/favicons/bing.png"),"&Bing");
    this->menu->addAction(QIcon(":/res/favicons/yahoo.png"),"&Yahoo");
    this->menu->addAction(QIcon(":/res/favicons/duckduckgo.png"),"&DuckDuckGo");
    this->search_btn->setMenu(this->menu);
    this->search_btn->setIcon(QIcon(":/res/drawables/search.png"));
    this->search_btn->setParent(this->text);
    this->search_btn->setFlat(true);
}

QLineEdit* SearchLineEdit::initialize(){
    createSearchLineEdit();
    return this->text;
}
