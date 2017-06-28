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

#include "permissions.h"
#include <QFile>
#include <QDir>
#include <QIODevice>
#include <QTextStream>
#include <QStringList>


PermissionDialog::PermissionDialog(){
    setLayout(vbox);
    setWindowTitle("Crusta : Permissions");

    vbox->addWidget(notif->group);
    vbox->addWidget(mouse->group);
    vbox->addWidget(geo->group);
    vbox->addWidget(audio->group);
    vbox->addWidget(video->group);
    vbox->addWidget(av->group);

    geo->allow->setChecked(true);
    setFixedSize(sizeHint().width()*2,sizeHint().height());

    QPushButton* ok=new QPushButton("Save");
    QPushButton* cncl=new QPushButton("Cancel");
    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel());
    hbox->addWidget(ok);
    hbox->addWidget(cncl);
    vbox->addLayout(hbox);
    loadPermissions();
    connect(cncl,&QPushButton::clicked,this,&PermissionDialog::close);
    connect(ok,&QPushButton::clicked,this,&PermissionDialog::savePermissions);
}

void PermissionDialog::savePermissions(){
    QString s="";
    if(notif->allow->isChecked())
        s+="0\n";
    else if(notif->ask->isChecked())
        s+="1\n";
    else
        s+="2\n";
    if(mouse->allow->isChecked())
        s+="0\n";
    else if(mouse->ask->isChecked())
        s+="1\n";
    else
        s+="2\n";
    if(geo->allow->isChecked())
        s+="0\n";
    else if(geo->ask->isChecked())
        s+="1\n";
    else
        s+="2\n";
    if(audio->allow->isChecked())
        s+="0\n";
    else if(audio->ask->isChecked())
        s+="1\n";
    else
        s+="2\n";
    if(video->allow->isChecked())
        s+="0\n";
    else if(video->ask->isChecked())
        s+="1\n";
    else
        s+="2\n";
    if(av->allow->isChecked())
        s+="0\n";
    else if(av->ask->isChecked())
        s+="1\n";
    else
        s+="2\n";
    QFile file(QDir::homePath()+"/.crusta_db/permissions.txt");
    file.open(QIODevice::WriteOnly);
    file.resize(0);
    QTextStream out(&file);
    out << s;
    file.close();
}

void PermissionDialog::loadPermissions(){
    QStringList slist;
    QFile f(QDir::homePath()+"/.crusta_db/permissions.txt");
    if(f.exists()){
        f.open(QIODevice::ReadOnly);
        QTextStream in(&f);
        while (!in.atEnd()){
           QString line = in.readLine();
           slist.append(line);
        }
        f.close();
    }
    else{
        return;
    }
    if(slist[0]=="0")
        notif->allow->setChecked(true);
    else if(slist[0]=="1")
        notif->ask->setChecked(true);
    else
        notif->reject->setChecked(true);
    if(slist[1]=="0")
        mouse->allow->setChecked(true);
    else if(slist[1]=="1")
        mouse->ask->setChecked(true);
    else
        mouse->reject->setChecked(true);
    if(slist[2]=="0")
        geo->allow->setChecked(true);
    else if(slist[2]=="1")
        geo->ask->setChecked(true);
    else
        geo->reject->setChecked(true);
    if(slist[3]=="0")
        audio->allow->setChecked(true);
    else if(slist[3]=="1")
        audio->ask->setChecked(true);
    else
        audio->reject->setChecked(true);
    if(slist[4]=="0")
        video->allow->setChecked(true);
    else if(slist[4]=="1")
        video->ask->setChecked(true);
    else
        video->reject->setChecked(true);
    if(slist[5]=="0")
        av->allow->setChecked(true);
    else if(slist[5]=="1")
        av->ask->setChecked(true);
    else
        av->reject->setChecked(true);
}

PermissionValue::PermissionValue(QString n){
    name=n;
    group->setTitle(name);
    QHBoxLayout* hbox=new QHBoxLayout();
    group->setLayout(hbox);
    hbox->addWidget(allow);
    hbox->addWidget(ask);
    hbox->addWidget(reject);
    ask->setChecked(true);
}
