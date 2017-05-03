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

#ifndef THEMEEDITOR_H
#define THEMEEDITOR_H

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QColorDialog>

class ThemeEditor:public QWidget{
public:
    QGroupBox* tab =new QGroupBox();
    QSpinBox* atabwidth=new QSpinBox();
    QSpinBox* ptabwidth=new QSpinBox();
    QLineEdit* atabcolor=new QLineEdit();
    QPushButton* atc=new QPushButton();
    QLineEdit* ptabcolor=new QLineEdit();
    QPushButton* ptc=new QPushButton();
    QLineEdit* atabhover=new QLineEdit();
    QPushButton* ath=new QPushButton();
    QLineEdit* ptabhover=new QLineEdit();
    QPushButton* pth=new QPushButton();

    QGroupBox* tabbar =new QGroupBox();
    QSpinBox* left=new QSpinBox();
    QLineEdit* color=new QLineEdit();
    QPushButton* tbc=new QPushButton();

    QVBoxLayout* vbox=new QVBoxLayout();

    ThemeEditor();
    void createTabTheme();
    void createTabBarTheme();
    void m_atc();
    void m_ptc();
    void m_ath();
    void m_pth();
    void m_tbc();
};

#endif // THEMEEDITOR_H
