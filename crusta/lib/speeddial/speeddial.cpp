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

#include "speeddial.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QWebEngineView>
#include <QSettings>

void SpeedDial::save(QString bgimage)
{
    QString upper;

    if (bgimage.startsWith("#")) {
        upper = "<!doctype html> <html> <head> <meta charset='utf-8'> <title>New Tab</title> <link rel='icon' href='img/favicon.ico' /> <style> body{background-color:" + bgimage + "; } .logo{width: 244px; height: 165.2px; margin: auto; } a{text-decoration: none; color: black; }.search-box{font-size: 16px; width: 100%; padding: 8px 8px; outline: none; } .box{width: 150px; height: 110px; display: inline-block; bckground-color: white; margin: 20px; position: relative; box-shadow: 0 3px 6px 0 rgba(0,0,0,0.2); transition: 0.3s; } .box:hover{box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2); } .box-image{width: 150px; height: 90px; background-color: #fff; background-position: center; background-size: cover; position: relative; display: block; } .box-btn{background: transparent; position:relative; float: right; outline: none; border: 0px; } .box-btn:hover{color: crimson; } .box-title{width: 150px; height: 20px; font-size: 14px; position: relative; display: block; background: white; font-family: \"Lucida Sans Unicode\", \"Arial Unicode MS\"; } .add-box{width: 150px; height: 90px; background-color: transparent; margin: 20px; position: relative; top: 10px; } .filter-btn{background-image: url('img/filter_uiYTal9087eEcrustakey.png'); background-position: center; padding: 15px; border: none; float: right; border-radius: 20px; background-color: transparent; outline: none; transition: 1s; } .filter-btn:hover{border-radius: 2px; background-color: #d0d0d0; } </style> </head> <body> <div style='width: 100%; display: flex; flex-direction: row-reverse;margin-top: 10px'> <button class='filter-btn' onclick='confirm(\"ifRiVOjJzQWe2MQ9h3xT_filter_btn\")'></button> </div> <div style='width: 80%; text-align: center; margin: auto; margin-top: 20px'>";
    } else {
        upper = "<!doctype html> <html> <head> <meta charset='utf-8'> <title>New Tab</title> <link rel='icon' href='img/favicon.ico' /> <style> body{background-color:#f0f0f0; background-image:url('" + bgimage + "');background-position: center; backgroud-repeat: no-repeat; background-size: cover;} .logo{width: 244px; height: 165.2px; margin: auto; } a{text-decoration: none; color: black; }.search-box{font-size: 16px; width: 100%; padding: 8px 8px; outline: none; } .box{width: 150px; height: 110px; display: inline-block; bckground-color: white; margin: 20px; position: relative; box-shadow: 0 3px 6px 0 rgba(0,0,0,0.2); transition: 0.3s; } .box:hover{box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2); } .box-image{width: 150px; height: 90px; background-color: #fff; background-position: center; background-size: cover; position: relative; display: block; } .box-btn{background: transparent; position:relative; float: right; outline: none; border: 0px; } .box-btn:hover{color: crimson; } .box-title{width: 150px; height: 20px; font-size: 14px; position: relative; display: block; background: white; font-family: \"Lucida Sans Unicode\", \"Arial Unicode MS\"; } .add-box{width: 150px; height: 90px; background-color: transparent; margin: 20px; position: relative; top: 10px; } .filter-btn{background-image: url('img/filter_uiYTal9087eEcrustakey.png'); background-position: center; padding: 15px; border: none; float: right; border-radius: 20px; background-color: transparent; outline: none; transition: 1s; } .filter-btn:hover{border-radius: 2px; background-color: #d0d0d0; } </style> </head> <body> <div style='width: 100%; display: flex; flex-direction: row-reverse;margin-top: 10px'> <button class='filter-btn' onclick='confirm(\"ifRiVOjJzQWe2MQ9h3xT_filter_btn\")'></button> </div> <div style='width: 80%; text-align: center; margin: auto; margin-top: 20px'>";
    }

    QString lower = "<img src='img/add.png' class='add-box' onclick='getNewBoxUrl()'></img> </div> <script> function getNewBoxUrl(){prompt('ifRiVOjEJzLc2MQ9h3xT')} </script> </body> </html>";
    QString middle = "";
    QFile inputFile(QDir::homePath() + "/.crusta_db/startpage.txt");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList data = line.split(">>>>>");

            if (!(data[0] == "" || data[1] == "")) {
                QFile imf(QDir::homePath() + "/.crusta_db/speeddial/img/" + data[0] + ".png");
                QString s;

                if (imf.exists()) {
                    s = "<a href='" + data[1] + "'> <div class='box'> <div class='box-image' style='background-image: url(\"img/" + data[0] + ".png\")'> <button class='box-btn' onclick='confirm(\"ifRiVOjEJzLc2MQ9h3xT " + data[0] + "\"); return false;'>&#x2715;</button> </div> <div class='box-title'>" + data[0] + "</div> </div> </a>";
                } else {
                    s = "<a href='" + data[1] + "'> <div class='box'> <div class='box-image' style='background-image: url(\"img/" + data[0] + ".png\")'> <button class='box-btn' onclick='confirm(\"ifRiVOjEJzLc2MQ9h3xT " + data[0] + "\"); return false;'>&#x2715;</button> </div> <div class='box-title'>" + data[0] + "</div> </div> </a>";
                }

                middle += s;
            }
        }

        inputFile.close();
    }

    QFile f(QDir::homePath() + "/.crusta_db" + "/speeddial/index.html");

    if (f.open(QIODevice::WriteOnly)) {
        QString s = upper + middle + lower;
        QTextStream t(&f);
        t.setCodec("UTF-8");
        f.resize(0);
        t << s;
        f.close();
    }
}

void SpeedDial::add(QString title, QString url)
{
    if (!(url.startsWith("http://") || url.startsWith("https://"))) {
        url = "http://" + url;
    }

    if (title.split(" ").length() > 1) {
        title = title.split(" ")[0];
    }

    QFile f(QDir::homePath() + "/.crusta_db/startpage.txt");

    if (f.open(QIODevice::Append)) {
        QTextStream t(&f);
        t.setCodec("UTF-8");
        t << title + ">>>>>" + url + "\n";
        f.close();
    }

    save(QSettings("Tarptaeya", "Crusta").value("speeddial_bgimage").toString());
}

void SpeedDial::remove(QString forbidden)
{
    QDir img;
    img.remove(QDir::homePath() + "/.crusta_db" + "/speeddial/img/" + forbidden + ".png");
    QFile f(QDir::homePath() + "/.crusta_db/startpage.txt");

    if (f.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QString s;
        QTextStream t(&f);
        t.setCodec("UTF-8");

        while (!t.atEnd()) {
            QString line = t.readLine();

            if (line.split(">>>>>")[0] != forbidden) {
                s.append(line + "\n");
            }
        }

        f.resize(0);
        t << s;
        f.close();
    }

    save(QSettings("Tarptaeya", "Crusta").value("speeddial_bgimage").toString());
}

void SpeedDial::configure()
{
    QString a = QCoreApplication::applicationDirPath() + "/speeddial/img/";
    QString b = QDir::homePath() + "/.crusta_db/speeddial/img/";
    QDir d(QCoreApplication::applicationDirPath() + "/speeddial/img/");
    QStringList filesList = d.entryList(QStringList("*"));

    for (QString file : filesList) {
        QFile::copy(a + file, b + file);
    }
}
