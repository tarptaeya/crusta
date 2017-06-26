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

#include "crustaspeech.h"
#include <QLabel>
#include <QVoice>

CrustaSpeech::CrustaSpeech(){
    if(speech->availableVoices().size()!=0)
        speech->setVoice(speech->availableVoices()[0]);
    setLayout(vbox);
    setWindowTitle(tr("Crusta Speech"));
    vbox->addWidget(tts);
    QHBoxLayout* hbox=new QHBoxLayout();
    hbox->addWidget(start_btn);
    hbox->addWidget(stop_btn);
    hbox->addWidget(new QLabel());
    vbox->addLayout(hbox);
    connect(start_btn,&QPushButton::clicked,this,&CrustaSpeech::speak);
    connect(stop_btn,&QPushButton::clicked,this,&CrustaSpeech::stop);
}

void CrustaSpeech::speak(){
    speech->say(tts->toPlainText());
    start_btn->setDisabled(true);
    stop_btn->setDisabled(false);
}

void CrustaSpeech::stop(){
    speech->stop();
    start_btn->setDisabled(false);
    stop_btn->setDisabled(true);
}

