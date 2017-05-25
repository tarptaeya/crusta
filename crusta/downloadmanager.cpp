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

#include "downloadmanager.h"
#include "downloadwidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QCoreApplication>
#include <QDir>
#include <QFileIconProvider>
#include <QTextStream>
#include <QFile>

#include <iostream>

DownloadManager::DownloadManager(){
    setLayout(vbox);
    vbox->addWidget(listwidget);
    vbox->addWidget(removeAll);
    loadDownloads();
}

void DownloadManager::addDownloadItem(DownloadWidget *w){
    QListWidgetItem* item=new QListWidgetItem();
    listwidget->insertItem(0,item);
    item->setSizeHint(w->size());
    listwidget->setItemWidget(item,w);
}

void DownloadManager::loadDownloads(){
    QDir* exec_dir=new QDir(QCoreApplication::applicationDirPath());
    exec_dir->cd("../crusta_db");
    QFile inputFile(exec_dir->absolutePath()+"/downloads.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          DownloadWidget* w=new DownloadWidget();
          w->getName(line.split('/')[line.split('/').length()-1]);
          QFileIconProvider* fip=new QFileIconProvider();
          QIcon icon=fip->icon(QFileInfo(line));
          w->getIcon(icon);
          w->path=line;
          w->changeLayout_Completed();
          w->setFixedHeight(w->sizeHint().height());
          this->addDownloadItem(w);
       }
       inputFile.close();
    }
}
