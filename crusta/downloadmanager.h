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


#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "downloadwidget.h"

#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListWidget>

class DownloadManager:public QDialog{
    Q_OBJECT
public:
    QVBoxLayout* vbox=new QVBoxLayout();
    QListWidget* listwidget=new QListWidget();
    DownloadManager();
    void addDownloadItem(DownloadWidget *w);
    void loadDownloads();
};

#endif // DOWNLOADMANAGER_H
