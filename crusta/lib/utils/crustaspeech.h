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

#ifndef CRUSTASPEECH_H
#define CRUSTASPEECH_H

#include <QTextToSpeech>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>

class CrustaSpeech:public QWidget{
    Q_OBJECT
    QVBoxLayout* vbox=new QVBoxLayout();
public:
    QTextEdit* tts=new QTextEdit();
    QTextToSpeech* speech=new QTextToSpeech();
    QPushButton* start_btn=new QPushButton(tr("Speak"));
    QPushButton* stop_btn=new QPushButton(tr("Stop"));
    CrustaSpeech();
    void speak();
    void stop();
};



#endif // CRUSTASPEECH_H
