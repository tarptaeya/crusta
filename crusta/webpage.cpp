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

#include "webpage.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QDir>
#include <QWebEngineProfile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>



Profile::Profile(){
    setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
}



QStringList WebPage::chooseFiles(FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes){
    QFileDialog* f=new QFileDialog();
    //f->setOption(QFileDialog::DontUseNativeDialog,true);
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
    QFile inputFile(QDir::homePath()+"/.crusta_db/settings.txt");
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
    preventAlert=false;
    alertCount=0;
    loadUAString();
}

bool WebPage::certificateError(const QWebEngineCertificateError &error){
    if (error.isOverridable()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(error.errorDescription());
        msgBox.setInformativeText(tr("If you wish so, you may continue with an unverified certificate. "
                                     "Accepting an unverified certificate means "
                                     "you may not be connected with the host you tried to connect to.\n"
                                     "Do you wish to override the security check and continue?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        return msgBox.exec() == QMessageBox::Yes;
    }
    QMessageBox::critical(view(), tr("Certificate Error"), error.errorDescription(), QMessageBox::Ok, QMessageBox::NoButton);
    return false;
}

void WebPage::javaScriptAlert(const QUrl &securityOrigin, const QString &msg){
    if(preventAlert) return;
    QDialog* jad=new QDialog();
    jad->setWindowFlag(Qt::FramelessWindowHint);
    QVBoxLayout* vbox=new QVBoxLayout();
    jad->setLayout(vbox);
    QLabel* murl=new QLabel(tr("Page at ")+securityOrigin.toString()+tr(" says:"));
    murl->setStyleSheet("font-size: 14px; font: italics");
    vbox->addWidget(murl);
    QLabel* mlbl=new QLabel(msg);
    mlbl->setStyleSheet("margin-top: 15px; margin-bottom: 15px;");
    vbox->addWidget(mlbl);
    if(alertCount>=1){
        QCheckBox* prvnt_btn=new QCheckBox(tr("Prevent this page from additional dialogs"));
        vbox->addWidget(prvnt_btn);
        connect(prvnt_btn,&QCheckBox::clicked,this,[this]{
            preventAlert=true;
        });
    }
    QPushButton* okbtn=new QPushButton(tr("OK"));
    okbtn->setFixedWidth(80);
    connect(okbtn,&QPushButton::clicked,jad,&QDialog::accept);
    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel());
    hbox->addWidget(okbtn);
    vbox->addLayout(hbox);
    jad->setObjectName("dialog");
    jad->setStyleSheet("#dialog{border: 1px solid #00b0e3}");
    alertCount++;
    jad->exec();
}



QStringList PrivateWebPage::chooseFiles(FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes){
    QFileDialog* f=new QFileDialog();
    //f->setOption(QFileDialog::DontUseNativeDialog,true);
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
    QFile inputFile(QDir::homePath()+"/.crusta_db/settings.txt");
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

PrivateWebPage::PrivateWebPage(QWebEngineProfile *profile, QObject *parent):QWebEnginePage(profile,parent){
    loadUAString();
}

bool PrivateWebPage::certificateError(const QWebEngineCertificateError &error){
    if (error.isOverridable()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(error.errorDescription());
        msgBox.setInformativeText(tr("If you wish so, you may continue with an unverified certificate. "
                                     "Accepting an unverified certificate means "
                                     "you may not be connected with the host you tried to connect to.\n"
                                     "Do you wish to override the security check and continue?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        return msgBox.exec() == QMessageBox::Yes;
    }
    QMessageBox::critical(view(), tr("Certificate Error"), error.errorDescription(), QMessageBox::Ok, QMessageBox::NoButton);
    return false;
}

void PrivateWebPage::javaScriptAlert(const QUrl &securityOrigin, const QString &msg){
    if(preventAlert) return;
    QDialog* jad=new QDialog();
    jad->setWindowFlag(Qt::FramelessWindowHint);
    QVBoxLayout* vbox=new QVBoxLayout();
    jad->setLayout(vbox);
    QLabel* murl=new QLabel(tr("Page at ")+securityOrigin.toString()+tr(" says:"));
    murl->setStyleSheet("font-size: 14px; font: italics");
    vbox->addWidget(murl);
    QLabel* mlbl=new QLabel(msg);
    mlbl->setStyleSheet("margin-top: 15px; margin-bottom: 15px;");
    vbox->addWidget(mlbl);
    if(alertCount>=1){
        QCheckBox* prvnt_btn=new QCheckBox(tr("Prevent this page from additional dialogs"));
        vbox->addWidget(prvnt_btn);
        connect(prvnt_btn,&QCheckBox::clicked,this,[this]{
            preventAlert=true;
        });
    }
    QPushButton* okbtn=new QPushButton(tr("OK"));
    okbtn->setFixedWidth(80);
    connect(okbtn,&QPushButton::clicked,jad,&QDialog::accept);
    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel());
    hbox->addWidget(okbtn);
    vbox->addLayout(hbox);
    jad->setObjectName("dialog");
    jad->setStyleSheet("#dialog{border: 1px solid #00b0e3}");
    alertCount++;
    jad->exec();
}
