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

#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>



class PermissionValue{
public:
    QString name;
    QGroupBox* group=new QGroupBox();
    QRadioButton* allow=new QRadioButton("Allow");
    QRadioButton* ask=new QRadioButton("Ask");
    QRadioButton* reject=new QRadioButton("Never");
    PermissionValue(QString name);
};



class PermissionDialog: public QWidget{
    QVBoxLayout* vbox=new QVBoxLayout();
    PermissionValue* notif=new PermissionValue("Notifications");
    PermissionValue* mouse=new PermissionValue("Mouse Lock");
    PermissionValue* geo=new PermissionValue("Geolocation");
    PermissionValue* audio=new PermissionValue("Audio Capture");
    PermissionValue* video=new PermissionValue("Video Capture");
    PermissionValue* av=new PermissionValue("Audio Video Capture");
public:
    PermissionDialog();
    void loadPermissions();
    void savePermissions();
};

#endif // PERMISSIONS_H
