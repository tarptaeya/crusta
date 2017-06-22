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

#ifndef BOOKMARKMANAGER_H
#define BOOKMARKMANAGER_H

#include "mainview.h"
#include "privatemainview.h"

#include <QDialog>
#include <QTreeWidget>
#include <QAction>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPoint>
#include <QLabel>


class BookmarkManager:public QDialog{
public:
    QTreeWidget* display=new QTreeWidget();
    QAction* open=new QAction(tr("Open"));
    QAction* del=new QAction(tr("Remove"));
    QLineEdit* search=new QLineEdit();
    QLineEdit* description=new QLineEdit();
    QPushButton* save=new QPushButton(tr("Save Description"));
    QPushButton* sbtn=new QPushButton(tr("Go"));
    QVBoxLayout* vbox=new QVBoxLayout();
    BookmarkManager(MainView* m);
    MainView* mview;
    void loadBookmarks();
    void showContextMenu(const QPoint& pos);
    void openUrl();
    void clearEntry();
    void saveDescription();
    void searchBookmark();
};

class PrivateBookmarkManager:public QDialog{
public:
    QTreeWidget* display=new QTreeWidget();
    QAction* open=new QAction(tr("Open"));
    QAction* del=new QAction(tr("Remove"));
    QLineEdit* search=new QLineEdit();
    QLineEdit* description=new QLineEdit();
    QPushButton* save=new QPushButton(tr("Save Description"));
    QPushButton* sbtn=new QPushButton(tr("Go"));
    QVBoxLayout* vbox=new QVBoxLayout();
    PrivateBookmarkManager(PrivateMainView* m);
    PrivateMainView* mview;
    void loadBookmarks();
    void showContextMenu(const QPoint& pos);
    void openUrl();
    void clearEntry();
    void saveDescription();
    void searchBookmark();
};

#endif // BOOKMARKMANAGER_H
