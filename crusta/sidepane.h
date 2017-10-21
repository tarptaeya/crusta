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

#ifndef SIDEPANE_H
#define SIDEPANE_H

#include "mainview.h"
#include "privatemainview.h"
#include "downloadmanager.h"
#include "historymanager.h"
#include "bookmarkmanager.h"

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWebEngineFullScreenRequest>


class SidePaneButton: public QPushButton{
public:
    int click;
    SidePaneButton();
    QString url;
    QString icon_;
    QWebEngineView* sidewebview=new QWebEngineView();
    void buttonContext(const QPoint &point);
};


class SidePane: public QWidget{
    QHBoxLayout* hbox=new QHBoxLayout();
    QVBoxLayout* vbox=new QVBoxLayout();
    QLabel* flexilabel=new QLabel();
public:
    SidePane(MainView*m);
    SidePane(PrivateMainView*m);
    PrivateMainView* pmainview;
    MainView* mainview;
    HistoryManager* history_manager;
    BookmarkManager* bookmark_manager;
    PrivateBookmarkManager* pbookmark_manager;
    DownloadManager* download_manager;
    SidePaneButton* history=new SidePaneButton();
    SidePaneButton* bookmarks=new SidePaneButton();
    SidePaneButton* downloads=new SidePaneButton();
    SidePaneButton* add_pane_btn=new SidePaneButton();
    void addNewButton();
    void acceptFullScreenReuest(QWebEngineFullScreenRequest request);
};


#endif // SIDEPANE_H
