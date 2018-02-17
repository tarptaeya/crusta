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



DownloadManager::DownloadManager()
{
    setLayout(vbox);
    setWindowTitle(tr("Crusta : Download Manager"));
    vbox->addWidget(listwidget);
    vbox->setContentsMargins(3, 10, 10, 10);
    loadDownloads();
    //setStyleSheet("QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
}

void DownloadManager::addDownloadItem(DownloadWidget *w)
{
    QListWidgetItem *item = new QListWidgetItem();
    listwidget->insertItem(0, item);
    item->setSizeHint(w->size());
    w->item = item;
    listwidget->setItemWidget(item, w);
    this->setMinimumWidth(w->width() + 45);
    //this->setMinimumHeight(w->height()*5);
}

void DownloadManager::loadDownloads()
{
    QFile inputFile(QDir::homePath() + "/.crusta_db/downloads.txt");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");

        while (!in.atEnd()) {
            QString line = in.readLine();
            DownloadWidget *w = new DownloadWidget();
            w->getName(line.split(">>>>>")[0].split('/')[line.split('/').length() - 1]);
            QFileIconProvider *fip = new QFileIconProvider();
            QIcon icon = fip->icon(QFileInfo(line.split(">>>>>")[0]));
            w->getIcon(icon);
            w->path = line;

            if (line.split(">>>>>").length() < 3) {
                continue;
            }

            if (line.split(">>>>>")[1] == "Completed") {
                w->changeLayout_Completed();
            } else if (line.split(">>>>>")[1] == "Canceled") {
                w->changeLayout_Canceled();
            } else {
                w->changeLayout_Interrupted();
            }

            w->setFixedHeight(w->sizeHint().height());
            this->addDownloadItem(w);
        }

        inputFile.close();
    }
}
