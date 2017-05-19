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

#include "jseditor.h"
#include <QString>

JsEditor::JsEditor(){
    vbox->addWidget(editor);
    run_btn->setText(tr("&Run Script"));
    cncl_btn->setText(tr("&Cancel"));
    hbox->addWidget(run_btn);
    hbox->addWidget(cncl_btn);
    vbox->addLayout(hbox);
    connect(run_btn,&QPushButton::clicked,this,&JsEditor::runJs);
    connect(cncl_btn,&QPushButton::clicked,this,&JsEditor::hide);
    setLayout(vbox);
    setStyleSheet("QWidget{background-color:blueviolet;} QPlainTextEdit{background-color:white;color:blue;} QPushButton{border:0.5px solid;background-color:crimson;color:white;border-radius:0px;border-color:crimson;padding:2px 4px;} QPushButton:hover{background-color:white;color:crimson}");
}

void JsEditor::setView(QWebEngineView* v){
    view=v;
}

void JsEditor::runJs(){
    QString code=editor->toPlainText();
    view->page()->runJavaScript(code);
    hide();
}
