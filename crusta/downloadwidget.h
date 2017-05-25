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

#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QProgressBar>
#include <QWebEngineDownloadItem>

class DownloadWidget:public QWidget {
public:
    QHBoxLayout* hbox=new QHBoxLayout();
    QVBoxLayout* v0box=new QVBoxLayout();
    QVBoxLayout* v1box=new QVBoxLayout();
    DownloadWidget();
    QLabel* icon=new QLabel();
    QLabel* name=new QLabel();
    QProgressBar* progress=new QProgressBar();
    QPushButton* cancel=new QPushButton();
    QPushButton* remove=new QPushButton();
    QPushButton* open=new QPushButton();
    QLabel* fraction=new QLabel();
    QString path;
    void getName(QString);
    void getIcon(QIcon);
    void computeFraction(qint64 bytesRecieved,qint64 bytesTotal);
    void changeLayout_Completed();
    void changeLayout_Canceled();
    void changeLayout_Interrupted();
    void stateChanged(QWebEngineDownloadItem::DownloadState state);
};

#endif // DOWNLOADWIDGET_H
