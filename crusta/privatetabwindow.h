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

#ifndef PRIVATETABWINDOW_H
#define PRIVATETABWINDOW_H

#include "privateaddresslineedit.h"
#include "privatewebview.h"

#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QPoint>
#include <QTime>



class PrivateTabWindow:public QObject{
    void viewHome();
    void updateAddrBar();
    void updateStar();
    void setHomePage();
    void homeContext(const QPoint& pos);
    QProgressBar* pbar=new QProgressBar();
    QTime loadStartTime;
public:
    QVBoxLayout* vbox=new QVBoxLayout();
    QWidget* tab=new QWidget();
    QPushButton* back_btn=new QPushButton();
    QPushButton* fwd_btn=new QPushButton();
    QPushButton* load_btn=new QPushButton();
    QPushButton* time_lbl=new QPushButton();
    QPushButton* home_btn=new QPushButton();
    QPushButton* bookmark_btn=new QPushButton();
    QPushButton* tool_btn=new QPushButton();
    QPushButton* menu_btn=new QPushButton();
    PrivateAddressLineEdit* addr_bar=new PrivateAddressLineEdit();
    PrivateWebView* view=new PrivateWebView();
    void createControls();
    QWidget* returnTab();
    QWidget* returnTab(PrivateWebView *view);
    PrivateTabWindow* returnThis();
    void setWebView(PrivateWebView*);
    void loadUrl();
    void bookmarkPage();
    void pageProgress(int p);
    void loadCompleted();
    void loadBegin();
    void showSiteInfo();
    void showLoadTime();
};



#endif // PRIVATETABWINDOW_H
