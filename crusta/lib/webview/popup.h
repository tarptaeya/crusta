/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <anmol@crustabrowser.com>
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

#ifndef POPUP_H
#define POPUP_H

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWidget>
#include <QIcon>
#include <QPushButton>




class PopUp:public QObject{
    Q_OBJECT
public:
    QWidget* widget=new QWidget();
    QPushButton* btn=new QPushButton();
    QLineEdit* lineedit=new QLineEdit();
    QVBoxLayout* vbox=new QVBoxLayout();
    QWebEngineView* webview =new QWebEngineView();
    void createView();
    void updateURL();
    void updateFavicon(QIcon icon);
    void updateTitle();
    void close();
    void setWebView(QWebEngineView*);
};




#endif // POPUP_H
