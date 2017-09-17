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

#include "themeeditor.h"
#include <QLabel>
#include <QDir>

ThemeEditor::ThemeEditor(){
    setWindowTitle("Crusta : Edit Theme");
    setLayout(vbox);
    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(new QLabel());
    hbox->addWidget(color);
    color->setPlaceholderText("enter a valid css color or hex code");
    hbox->addWidget(cdg);
    cdg->setText(tr("Pick Color"));
    connect(cdg,&QPushButton::clicked,this,&ThemeEditor::setColor);
    hbox->addWidget(new QLabel());
    vbox->addLayout(hbox);
    QHBoxLayout* h1box=new QHBoxLayout();
    h1box->addWidget(restore);
    restore->setText(tr("Restore"));
    connect(restore,&QPushButton::clicked,this,&ThemeEditor::restoreTheme);
    h1box->addWidget(new QLabel());
    h1box->addWidget(cncl);
    cncl->setText(tr("Cancel"));
    connect(cncl,&QPushButton::clicked,this,&QDialog::reject);
    h1box->addWidget(ok);
    ok->setText(tr("OK"));
    connect(ok,&QPushButton::clicked,this,&ThemeEditor::saveTheme);
    vbox->addLayout(h1box);
    setFixedWidth(500);
    //setStyleSheet("QDialog{background-color:white;color:black} QLabel{color:black} QLineEdit{padding:3px 8px;color:black;background-color:white;border: 1px solid black} QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
}

void ThemeEditor::restoreTheme(){
    this->accept();
    QString  _col="#00b0e3";
    QString theme="QTabWidget::tab-bar{left:0px;height:32} QTabBar{background-color:"+_col+";} QTabBar::tab:selected{background-color:white;color:black;max-width:175px;min-width:175px;height:32px} QTabBar::tab:!selected{max-width:173px;min-width:173px;color:black;background-color:#dbdbdb;top:2px;border:0.5px solid "+_col+";height:30px} QPushButton{border: none;background-color:#dbdbdb;} QPushButton:hover{background-color:white;}";
    QFile f(QDir::homePath()+"/.crusta_db/preference.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(line.split(">>>>>")[0]!="theme")
                s.append(line + "\n");
            else{
                s.append(line.split(">>>>>")[0]+">>>>>"+theme+"\n");
            }
        }
        f.resize(0);
        t << s;
        f.close();
    }
    _parent->loadTheme();
}

void ThemeEditor::saveTheme(){
    this->accept();
    QString  _col=color->text();
    QString theme="QTabWidget::tab-bar{left:0px;height:32} QTabBar{background-color:"+_col+";} QTabBar::tab:selected{background-color:white;color:black;max-width:175px;min-width:175px;height:32px} QTabBar::tab:!selected{max-width:173px;min-width:173px;color:black;background-color:#dbdbdb;top:2px;border:0.5px solid "+_col+";height:30px} QPushButton{border: none;background-color:#dbdbdb;} QPushButton:hover{background-color:white;}";
    QFile f(QDir::homePath()+"/.crusta_db/preference.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(line.split(">>>>>")[0]!="theme")
                s.append(line + "\n");
            else{
                s.append(line.split(">>>>>")[0]+">>>>>"+theme+"\n");
            }
        }
        f.resize(0);
        t << s;
        f.close();
    }
    _parent->loadTheme();
}

void ThemeEditor::setColor(){
    QColorDialog* cd=new QColorDialog();
    cd->setOption(QColorDialog::DontUseNativeDialog,true);
    QColor colour=cd->getColor(Qt::white,this,QString(tr("Crusta : pick-color")),cd->options());
    if(colour.isValid()){
        color->setText(colour.name());
    }
}
