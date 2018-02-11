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

#ifndef JSEDITOR_H
#define JSEDITOR_H

#include "codeeditor.h"

#include <QPlainTextEdit>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QObject>
#include <QWidget>

class JsEditor:public QWidget{
    Q_OBJECT
public:
    CodeEditor* editor=new CodeEditor();
    QVBoxLayout* vbox=new QVBoxLayout();
    QHBoxLayout* hbox=new QHBoxLayout();
    QPushButton* run_btn=new QPushButton();
    QPushButton* cncl_btn=new QPushButton();
    QWebEngineView* view=new QWebEngineView();
    JsEditor();
    void setView(QWebEngineView* v);
    void runJs();
};


#endif // JSEDITOR_H
