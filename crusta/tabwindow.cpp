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

#include "tabwindow.h"
#include "webview.h"

#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>

#include <cstdlib>



void TabWindow::viewHome(){
    this->view->returnView()->load(QUrl(this->view->home_page));
}

void TabWindow::updateAddrBar(){
    try{
    this->addr_bar->initialize()->setText(this->view->returnView()->url().toString());
    this->addr_bar->initialize()->setCursorPosition(0);
    }
    catch(...){
        return;
    }
}

void TabWindow::createControls(){
    QHBoxLayout* hbox=new QHBoxLayout();
    this->back_btn->setFlat(true);
    this->back_btn->setIcon(QIcon(":/res/drawables/back_btn.png"));
    connect(this->back_btn,&QPushButton::clicked,this->view->returnView(),&QWebEngineView::back);
    hbox->addWidget(this->back_btn);
    this->fwd_btn->setFlat(true);
    this->fwd_btn->setIcon(QIcon(":/res/drawables/fwd_btn.png"));
    connect(this->fwd_btn,&QPushButton::clicked,this->view->returnView(),&QWebEngineView::forward);
    hbox->addWidget(this->fwd_btn);
    this->load_btn->setFlat(true);
    this->load_btn->setIcon(QIcon(":/res/drawables/load_btn.png"));
    connect(this->load_btn,&QPushButton::clicked,this->view->returnView(),&QWebEngineView::reload);
    hbox->addWidget(this->load_btn);
    hbox->addWidget(this->addr_bar->initialize());
    connect(this->view->returnView(),&QWebEngineView::urlChanged,this,&TabWindow::updateAddrBar);
    hbox->addWidget(this->search_bar->initialize());
    this->home_btn->setFlat(true);
    this->home_btn->setIcon(QIcon(":/res/drawables/home_btn.png"));
    connect(this->home_btn,&QPushButton::clicked,this,&TabWindow::viewHome);
    hbox->addWidget(this->home_btn);
    this->bookmark_btn->setFlat(true);
    this->bookmark_btn->setIcon(QIcon(":/res/drawables/bookmark_btn.png"));
    hbox->addWidget(this->bookmark_btn);
    this->tool_btn->setFlat(true);
    this->tool_btn->setIcon(QIcon(":/res/drawables/tool_btn.png"));
    hbox->addWidget(this->tool_btn);
    this->options_btn->setFlat(true);
    this->options_btn->setIcon(QIcon(":/res/drawables/options_btn.png"));
    hbox->addWidget(this->options_btn);
    vbox->addLayout(hbox);
    tab->setLayout(vbox);
}

QWidget* TabWindow::returnTab(){
    this->vbox->setContentsMargins(0,0,0,0);
    createControls();
    this->vbox->addWidget(this->view->returnView());
    return this->tab;
}

TabWindow* TabWindow::returnThis(){
    this->vbox->setContentsMargins(0,0,0,0);
    createControls();
    return this;
}

void TabWindow::setWebView(WebView* v){
    view=v;
    vbox->addWidget(view);
}
