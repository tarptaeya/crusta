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

#include "themeeditor.h"
#include <QVBoxLayout>

void ThemeEditor::createTabTheme(){
    QVBoxLayout* tbox =new QVBoxLayout();
    tab->setLayout(tbox);
    tab->setTitle("Tabs");

    QHBoxLayout* t0box=new QHBoxLayout();
    t0box->addWidget(new QLabel(tr("active tab width")));
    t0box->addWidget(atabwidth);
    atabwidth->setMaximum(300);
    atabwidth->setMinimum(75);
    atabwidth->setSingleStep(1);
    atabwidth->setValue(175);
    tbox->addLayout(t0box);

    QHBoxLayout* t1box=new QHBoxLayout();
    t1box->addWidget(new QLabel(tr("passive tab width")));
    t1box->addWidget(ptabwidth);
    ptabwidth->setMaximum(300);
    ptabwidth->setMinimum(75);
    ptabwidth->setSingleStep(1);
    ptabwidth->setValue(175);
    tbox->addLayout(t1box);

    QHBoxLayout* t2box=new QHBoxLayout();
    t2box->addWidget(new QLabel(tr("active tab color")));
    t2box->addWidget(atabcolor);
    t2box->addWidget(atc);
    tbox->addLayout(t2box);

    QHBoxLayout* t3box=new QHBoxLayout();
    t3box->addWidget(new QLabel(tr("passive tab color")));
    t3box->addWidget(ptabcolor);
    t3box->addWidget(ptc);
    tbox->addLayout(t3box);

    QHBoxLayout* t4box=new QHBoxLayout();
    t4box->addWidget(new QLabel(tr("active tab hovered")));
    t4box->addWidget(atabhover);
    t4box->addWidget(ath);
    tbox->addLayout(t4box);

    QHBoxLayout* t5box=new QHBoxLayout();
    t5box->addWidget(new QLabel(tr("passive tab hovered")));
    t5box->addWidget(ptabhover);
    t5box->addWidget(pth);
    tbox->addLayout(t5box);

    vbox->addWidget(tab);
}

void ThemeEditor::createTabBarTheme(){
    QVBoxLayout* tbbox=new QVBoxLayout();
    tabbar->setLayout(tbbox);
    tabbar->setTitle(tr("Window"));

    QHBoxLayout* tb0box=new QHBoxLayout();
    tb0box->addWidget(new QLabel(tr("left")));
    tb0box->addWidget(left);
    left->setMaximum(100);
    left->setMinimum(0);
    left->setSingleStep(1);
    left->setValue(35);
    tbbox->addLayout(tb0box);

    QHBoxLayout* tb1box=new QHBoxLayout();
    tb1box->addWidget(new QLabel(tr("color")));
    tb1box->addWidget(color);
    tb1box->addWidget(tbc);
    tbbox->addLayout(tb1box);

    vbox->addWidget(tabbar);
}

ThemeEditor::ThemeEditor(){
    this->setLayout(vbox);
    createTabTheme();
    createTabBarTheme();
    connect(atc,&QPushButton::clicked,this,&ThemeEditor::m_atc);
    connect(ptc,&QPushButton::clicked,this,&ThemeEditor::m_ptc);
    connect(ath,&QPushButton::clicked,this,&ThemeEditor::m_ath);
    connect(pth,&QPushButton::clicked,this,&ThemeEditor::m_pth);
    connect(tbc,&QPushButton::clicked,this,&ThemeEditor::m_tbc);
}

void ThemeEditor::m_atc(){
    QColorDialog* cd=new QColorDialog();
    cd->setOption(QColorDialog::DontUseNativeDialog,true);
    QColor colour=cd->getColor(Qt::white,this,QString(tr("Crusta : pick-color")),cd->options());
    if(colour.isValid()){
        atabcolor->setText(colour.name());
    }
}

void ThemeEditor::m_ptc(){
    QColorDialog* cd=new QColorDialog();
    cd->setOption(QColorDialog::DontUseNativeDialog,true);
    QColor colour=cd->getColor(Qt::white,this,QString(tr("Crusta : pick-color")),cd->options());
    if(colour.isValid()){
        ptabcolor->setText(colour.name());
    }
}

void ThemeEditor::m_ath(){
    QColorDialog* cd=new QColorDialog();
    cd->setOption(QColorDialog::DontUseNativeDialog,true);
    QColor colour=cd->getColor(Qt::white,this,QString(tr("Crusta : pick-color")),cd->options());
    if(colour.isValid()){
        atabhover->setText(colour.name());
    }
}

void ThemeEditor::m_pth(){
    QColorDialog* cd=new QColorDialog();
    cd->setOption(QColorDialog::DontUseNativeDialog,true);
    QColor colour=cd->getColor(Qt::white,this,QString(tr("Crusta : pick-color")),cd->options());
    if(colour.isValid()){
        ptabhover->setText(colour.name());
    }
}

void ThemeEditor::m_tbc(){
    QColorDialog* cd=new QColorDialog();
    cd->setOption(QColorDialog::DontUseNativeDialog,true);
    QColor colour=cd->getColor(Qt::white,this,QString(tr("Crusta : pick-color")),cd->options());
    if(colour.isValid()){
        color->setText(colour.name());
    }
}




