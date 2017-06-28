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

#include "popup.h"

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWidget>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>



void PopUp::setWebView(QWebEngineView * view){
    webview=view;
}

void PopUp::createView(){
    widget->setLayout(vbox);
    btn->setParent(lineedit);
    btn->setFlat(true);
    btn->setFixedWidth(20);
    vbox->setContentsMargins(0,0,0,0);
    vbox->addWidget(lineedit);
    vbox->addWidget(webview);
    connect(webview,&QWebEngineView::urlChanged,this,&PopUp::updateURL);
    connect(webview,&QWebEngineView::iconChanged,this,&PopUp::updateFavicon);
    connect(webview,&QWebEngineView::titleChanged,this,&PopUp::updateTitle);
}

void PopUp::updateURL(){
    lineedit->setTextMargins(20,0,0,0);
    lineedit->setText(webview->url().toString());
    lineedit->setCursorPosition(0);
    lineedit->setReadOnly(true);
}

void PopUp::updateFavicon(QIcon icon){
    btn->setIcon(icon);
}

void PopUp::updateTitle(){
    widget->setWindowTitle(webview->title());
}

void PopUp::close(){
    widget->deleteLater();
}
