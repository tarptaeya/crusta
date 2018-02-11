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

#ifndef THEMEEDITOR_H
#define THEMEEDITOR_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QColorDialog>
#include "mainview.h"


class ThemeEditor:public QDialog{
    Q_OBJECT
private:
    QLineEdit* color=new QLineEdit();
    QVBoxLayout* vbox=new QVBoxLayout();
    QPushButton* restore=new QPushButton();
    QPushButton* ok=new QPushButton();
    QPushButton* cncl=new QPushButton();
    QPushButton* cdg=new QPushButton();
    void saveTheme();
    void restoreTheme();
    void setColor();
public:
    ThemeEditor();
    MainView* _parent=new MainView();
};



#endif // THEMEEDITOR_H
