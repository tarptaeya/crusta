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

#include "webpage.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QWebEngineProfile>

#include <iostream>



Profile::Profile(){
    setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
}



QStringList WebPage::chooseFiles(FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes){
    QFileDialog* f=new QFileDialog();
    f->setOption(QFileDialog::DontUseNativeDialog,true);
    switch(mode){
    case(QWebEnginePage::FileSelectOpen):{
        QString name=f->getOpenFileName(nullptr,QString(tr("Crusta : Upload File")),QDir::homePath(),QString(),nullptr,f->options());
        QStringList list;
        list.append(name);
        return list;
    }break;
    case(QWebEnginePage::FileSelectOpenMultiple):{
        QStringList names=f->getOpenFileNames(nullptr,QString(tr("Crusta : Upload Multiple Files")),QDir::homePath(),QString(),nullptr,f->options());
        return names;
    }break;
    }
    return QStringList();
}

void WebPage::loadUAString(){
    QFile inputFile("preference.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data[0]=="UA String"){
              agent=data[1];
              break;
          }
       }
       inputFile.close();
    }
    profile()->setHttpUserAgent(agent);
}

WebPage::WebPage(){
    loadUAString();
}



QStringList PrivateWebPage::chooseFiles(FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes){
    QFileDialog* f=new QFileDialog();
    f->setOption(QFileDialog::DontUseNativeDialog,true);
    switch(mode){
    case(QWebEnginePage::FileSelectOpen):{
        QString name=f->getOpenFileName(nullptr,QString(tr("Crusta : Upload File")),QDir::homePath(),QString(),nullptr,f->options());
        QStringList list;
        list.append(name);
        return list;
    }break;
    case(QWebEnginePage::FileSelectOpenMultiple):{
        QStringList names=f->getOpenFileNames(nullptr,QString(tr("Crusta : Upload Multiple Files")),QDir::homePath(),QString(),nullptr,f->options());
        return names;
    }break;
    }
    return QStringList();
}

void PrivateWebPage::loadUAString(){
    QFile inputFile("preference.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data[0]=="Incognito UA String"){
              agent=data[1];
              break;
          }
       }
       inputFile.close();
    }
    profile()->setHttpUserAgent(agent);
}

PrivateWebPage::PrivateWebPage(QWebEngineProfile *profile, QObject *parent):QWebEnginePage(profile,parent){
    loadUAString();
}
