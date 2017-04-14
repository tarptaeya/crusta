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

#include "mainview.h"
#include <QIcon>
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    a.setApplicationName(QString("Crusta"));
    a.setApplicationVersion(QString("1.0"));
    Q_INIT_RESOURCE(resource); // initialised the resource file;
    MainView* w=new MainView();
    w->showView();
    a.setWindowIcon(QIcon(":/res/drawables/icon_3.ico"));
    return a.exec();
}
