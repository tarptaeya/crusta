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
#include <QIODevice>
#include <QDir>
#include <QStringList>
#include <QCoreApplication>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QDialog>
#include <QPixmap>
#include <QUrl>
#include <QWebEngineView>
#include <QSettings>

#include <iostream>

void SpeedDial::save(QString bgimage, QString srchstr){
    QString upper;
    if(bgimage.startsWith("#")){
        upper="<!doctype html> <html> <head> <meta charset='utf-8'> <title>New Tab</title> <link rel='icon' href='img/favicon.ico' /> <style> body{background-color:"+bgimage+"; } .logo{width: 244px; height: 165.2px; margin: auto; } a{text-decoration: none; color: black; }.search-box{font-size: 16px; width: 100%; padding: 8px 8px; outline: none; } .box{width: 150px; height: 110px; display: inline-block; bckground-color: white; margin: 20px; position: relative; box-shadow: 0 3px 6px 0 rgba(0,0,0,0.2); transition: 0.3s; } .box:hover{box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2); } .box-image{width: 150px; height: 90px; background-color: #fff; background-position: center; background-size: cover; position: relative; display: block; } .box-btn{background: transparent; position:relative; float: right; outline: none; border: 0px; } .box-btn:hover{color: crimson; } .box-title{width: 150px; height: 20px; font-size: 14px; position: relative; display: block; background: white; font-family: \"Lucida Sans Unicode\", \"Arial Unicode MS\"; } .add-box{width: 150px; height: 90px; background-color: transparent; margin: 20px; position: relative; top: 10px; } .filter-btn{background-image: url('img/filter_uiYTal9087eEcrustakey.png'); background-position: center; padding: 15px; border: none; float: right; border-radius: 20px; background-color: transparent; outline: none; transition: 1s; } .filter-btn:hover{border-radius: 2px; background-color: #d0d0d0; } </style> </head> <body> <div style='width: 100%; display: flex; flex-direction: row-reverse;margin-top: 10px'> <button class='filter-btn' onclick='confirm(\"ifRiVOjJzQWe2MQ9h3xT_filter_btn\")'></button> </div> <div style='width: 50%; text-align: center; margin: auto; margin-top: 20px'>";
    }else{
        upper="<!doctype html> <html> <head> <meta charset='utf-8'> <title>New Tab</title> <link rel='icon' href='img/favicon.ico' /> <style> body{background-color:#f0f0f0; background-image:url('"+bgimage+"');background-position: center; backgroud-repeat: no-repeat; background-size: cover;} .logo{width: 244px; height: 165.2px; margin: auto; } a{text-decoration: none; color: black; }.search-box{font-size: 16px; width: 100%; padding: 8px 8px; outline: none; } .box{width: 150px; height: 110px; display: inline-block; bckground-color: white; margin: 20px; position: relative; box-shadow: 0 3px 6px 0 rgba(0,0,0,0.2); transition: 0.3s; } .box:hover{box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2); } .box-image{width: 150px; height: 90px; background-color: #fff; background-position: center; background-size: cover; position: relative; display: block; } .box-btn{background: transparent; position:relative; float: right; outline: none; border: 0px; } .box-btn:hover{color: crimson; } .box-title{width: 150px; height: 20px; font-size: 14px; position: relative; display: block; background: white; font-family: \"Lucida Sans Unicode\", \"Arial Unicode MS\"; } .add-box{width: 150px; height: 90px; background-color: transparent; margin: 20px; position: relative; top: 10px; } .filter-btn{background-image: url('img/filter_uiYTal9087eEcrustakey.png'); background-position: center; padding: 15px; border: none; float: right; border-radius: 20px; background-color: transparent; outline: none; transition: 1s; } .filter-btn:hover{border-radius: 2px; background-color: #d0d0d0; } </style> </head> <body> <div style='width: 100%; display: flex; flex-direction: row-reverse;margin-top: 10px'> <button class='filter-btn' onclick='confirm(\"ifRiVOjJzQWe2MQ9h3xT_filter_btn\")'></button> </div> <div style='width: 50%; text-align: center; margin: auto; margin-top: 20px'>";
    }
    if(srchstr=="Ecosia"){
        upper+="<svg focusable='false' xmlns='http://www.w3.org/2000/svg' class='logo' width='612' height='413' viewBox='0 196.5 612 413' enable-background='new 0 196.5 612 413'><g><path fill='#36ACB8' d='M409.954 482.12c-11.727-7.818-17.59-3.91-17.59-3.91s.244 9.04 9.04 16.614c8.795 7.573 20.52 6.596 20.52 6.596s-.242-11.482-11.97-19.3zM234.54 527.562c-8.308 14.17-4.643 26.142-4.643 26.142s9.528 2.688 21.5-12.948c11.97-15.637 7.085-31.028 7.085-31.028s-15.636 3.664-23.943 17.834zm-36.892-35.67c-8.795 7.574-9.04 16.614-9.04 16.614s5.863 3.908 17.59-3.91c11.727-7.817 11.97-19.3 11.97-19.3s-11.48-1.222-20.52 6.597zm108.474 31.272s-9.528 7.33-10.75 22.965c-1.222 15.636 9.284 27.118 9.284 27.118s12.216-9.284 11.238-28.096c-.732-19.056-9.772-21.988-9.772-21.988zm63.766-.244s-2.688 8.55 3.176 18.567 17.346 12.704 17.346 12.704 3.42-10.993-5.13-21.987c-8.797-11.238-15.392-9.283-15.392-9.283zm40.066-216.216c11.728-7.818 11.972-19.3 11.972-19.3s-11.482-1.222-20.522 6.596c-8.795 7.574-9.04 16.613-9.04 16.613s5.864 3.91 17.59-3.91zm-205.71 15.392s-.244-11.482-11.97-19.3c-11.728-7.818-17.59-3.91-17.59-3.91s.243 9.04 9.04 16.614c8.794 7.817 20.52 6.596 20.52 6.596zm38.846-42.51s3.42-10.995-5.13-21.99c-8.55-11.237-15.392-9.283-15.392-9.283s-2.688 8.55 3.176 18.567c6.108 10.263 17.346 12.705 17.346 12.705zm132.417-13.682c11.97-15.392 7.085-30.54 7.085-30.54s-15.636 3.666-23.942 17.59-4.397 25.898-4.397 25.898 9.282 2.688 21.254-12.948zm-71.584-13.68c.733-17.347-10.505-25.898-10.505-25.898s-9.528 10.505-8.55 24.92c1.22 14.414 10.016 21.01 10.016 21.01s8.306-2.687 9.04-20.033z'/><path fill='#DC7E58' d='M307.1 304.994c-50.084 0-90.64 40.556-90.64 90.64 0 50.083 40.556 90.64 90.64 90.64s90.64-40.557 90.64-90.64c0-50.084-40.556-90.64-90.64-90.64zm0 170.774c-44.22 0-79.89-35.914-79.89-80.134S262.88 315.5 307.1 315.5s79.89 35.914 79.89 80.134-35.67 80.134-79.89 80.134z'/><path fill='#36ACB8' d='M308.076 329.914c-36.89 0-66.452 30.05-66.452 66.453 0 36.892 29.562 66.453 66.452 66.453 36.647 0 66.21-29.807 66.21-66.453s-29.562-66.453-66.21-66.453z'/><g fill='#4A4A4A'><path d='M102.855 484.808L75.98 372.668l48.13-11.483 4.153 17.102-31.272 7.574 7.33 30.54 22.72-5.375 4.154 17.102-22.72 5.375 7.33 30.54 31.27-7.575 4.154 17.103-48.373 11.238zM185.92 468.438c-7.573 1.222-14.414-.244-20.52-4.642-6.11-4.398-9.773-10.262-11.24-17.835l-10.504-62.054c-1.222-7.574.244-14.415 4.642-20.522 4.398-6.108 10.505-10.017 17.835-11.238 7.574-1.222 14.414.245 20.522 4.642s9.772 10.505 10.994 17.834l2.198 12.95-17.834 2.93-2.2-13.436c-.487-2.688-1.71-4.886-4.152-6.597-2.2-1.71-4.642-2.2-7.573-1.71-2.69.488-4.887 1.71-6.353 4.152-1.466 2.2-2.2 4.642-1.71 7.574l10.994 62.544c.49 2.688 1.71 4.886 3.91 6.352 2.2 1.466 4.642 2.2 7.33 1.71 2.688-.488 4.886-1.71 6.596-3.908 1.71-2.2 2.2-4.643 1.71-7.33l-2.688-15.636 17.834-2.932 2.688 15.88c1.222 7.574-.244 14.415-4.642 20.522-4.644 5.864-10.508 9.53-17.837 10.75zM461.016 439.854c-.978 8.062-4.153 14.415-9.284 19.3-3.42 3.422-7.817 5.376-12.948 6.354-2.932.488-6.107.488-9.772-.244-6.84-.978-11.972-3.665-15.88-7.574-3.177-3.176-5.62-7.33-7.085-12.46-1.466-5.13-1.955-10.505-1.222-16.613l16.613 1.222c-.245 6.84.732 11.727 3.176 15.146 1.71 2.688 3.91 3.91 6.597 4.397 3.665.49 7.086-.978 9.773-4.153 1.466-1.466 2.442-3.91 2.932-7.33.732-4.64-.733-9.77-4.397-15.146-2.933-3.664-7.33-9.283-12.95-16.857-4.885-6.352-8.06-11.97-9.77-16.613-1.71-5.375-2.2-10.75-1.467-16.613 1.467-10.26 6.354-17.59 14.17-21.987 4.888-2.443 10.263-3.42 16.614-2.443 6.108.977 11.238 3.176 15.147 6.596 3.177 2.688 5.376 6.108 6.842 10.26s2.2 8.796 1.71 13.682l-16.857.49c.244-4.643-.488-8.552-2.443-11.483-1.22-2.2-3.42-3.42-6.352-3.91-3.176-.488-5.863.49-7.818 3.177-1.71 1.955-2.688 4.642-3.176 8.062-.733 5.13.733 10.75 4.397 16.857 1.466 2.2 3.665 5.132 6.597 8.552 3.42 3.91 5.863 6.597 6.84 8.307 3.665 4.886 6.108 9.527 7.818 13.925.733 2.2 1.466 3.91 1.71 5.863.488 4.395.976 8.06.488 11.236zM481.538 473.813l-17.103-3.42 23.21-112.873 17.102 3.42-23.21 112.873zM517.94 455l-10.75 24.677-17.59-4.397 47.64-106.52 23.21 5.863-10.017 116.292-17.59-4.642 2.687-26.63L517.94 455zm24.43-58.145l-17.59 42.266 12.46 3.177 5.13-45.442z'/></g></g><g><path fill='#D4DB34' d='M372.82 409.56c0 .488 0 .977.243 1.466.244-.733.244-1.466.488-2.443-.487.243-.487.73-.73.977zM327.133 421.774c-2.198 1.466-3.664 2.933-4.153 6.597.49.734.733 1.223 1.71 1.71 2.688-.976 5.62-5.373 3.665-8.06-.49-.246-.734-.246-1.222-.246zM295.373 337.487c-1.222-.488-2.688-.733-3.91-1.222-3.175.733-7.084 0-10.75-.733-2.687 1.222-5.374 2.688-7.817 4.153.488.49.977 1.222 1.22 1.955-.487 1.71-2.198 3.42-1.465 5.375.246 1.222.734 1.71 1.71 2.443 3.177.245 4.887-1.71 7.33-2.443 2.443-.49 5.13-.733 7.574-1.222 2.932-.732 8.063-4.885 6.11-8.306zM256.527 398.565c-1.222-1.71-2.443-3.665-3.665-5.375-3.176-3.176-6.352-5.863-10.505-6.84-.49 3.175-.733 6.596-.733 10.016 0 11.238 2.688 21.988 7.818 31.272.733-.733 1.466-1.222 2.2-1.955.977-1.71 2.198-3.42 3.175-5.13 1.71-1.71 4.397-2.2 5.863-4.153.49-1.466 1.222-3.177 1.71-4.643.977-1.954 3.42-3.42 2.932-6.107-1.22-3.91-6.108-4.397-8.795-7.085zM257.993 357.52c2.2-.976 3.42 1.71 5.13-.243 2.2-2.2-.732-5.864-.732-8.307 0-.49.245-1.222.49-1.71-4.398 3.91-8.063 8.55-11.24 13.438.98-.245 1.956-.245 2.69-.49 1.464-.732 2.44-1.71 3.663-2.687zM355.962 350.19c-1.466 0-2.932 0-4.397.246-6.597.977-13.193 1.955-20.767 2.2-6.597 0-10.017-5.376-16.37-3.91-2.442.49-7.084 3.176-8.306 5.13-1.71 2.69.488 4.398-2.688 5.864-2.2-.49-2.932-2.2-4.886-3.176-1.71.244-2.443.733-3.42 1.954-.244 2.932 1.955 3.42 1.955 6.352-1.22 1.71-4.153 1.955-4.886 3.91-.977 2.198 0 4.64-1.466 6.35-2.198 1.956-4.396 4.154-6.35 6.11-4.154 5.13-2.2 13.436.976 16.612 2.932 2.932 7.33.49 11.24 2.2 2.93 1.22 3.42 6.84 4.152 9.527 1.466 4.396-1.22 6.35-1.22 10.504s1.465 16.125 7.33 15.88c6.595-.244 8.06-6.84 11.726-11.237 1.222-1.467 3.176-2.2 4.153-4.398 0-2.2.244-4.643.244-6.84 1.467-3.91 6.11-5.132 8.552-8.063 2.443-2.688.244-4.887 1.466-7.085 0 0 .245 0 .245-.245 1.954-1.22 3.908-2.198 6.107-3.42.978-1.222 1.954-2.443 2.932-3.42.733-.244 1.466-.49 2.2-.733 7.572 3.91 4.885 11.727 10.26 15.88.488-.244.732-.488 1.222-.732.732-.978.244-1.955.732-3.42.978-2.933 4.397-5.864 8.062-5.13 1.71 2.197 4.642 5.13 5.13 8.306.49 2.932-1.465 4.642-1.465 7.085 0 1.71.977 3.91 2.442 6.107.978-2.443 1.71-5.13 2.2-7.817 0-.49 0-.978-.245-1.467.244-.244.244-.488.488-.977.733-3.91 1.222-8.063 1.222-12.217 0-17.59-7.086-33.96-18.568-45.932zm-36.402 30.54c-1.466.244-2.932-.733-3.91-1.222-1.71-.244-3.664-.244-5.374-.488-.244 0-8.795-6.84-9.04-7.33.49-1.222.978-1.222 1.71-1.71 3.176.733 3.91 2.688 6.107 3.665 1.222.244 2.2.49 3.42.733 1.222.977 2.443 1.71 3.665 2.688 1.466.733 3.42.733 4.643.977 0 .245.245.49.245.733v1.222c-.733.244-.977.488-1.465.732zM310.03 379.02l-9.04-7.33c.49.49 8.797 7.33 9.04 7.33z'/></g></svg><form method='GET' action='https://ecosia.org/search'> <input type='hidden' name='tt' value='crusta' /> <input type='text' class='search-box' name='q' autofocus autocomplete='on' placeholder='Search the web with Ecosia' /> </form> </div> <div style='text-align: center;width: 80%;margin: auto; margin-top: 50px'>";
    } else if(srchstr=="Yandex"){
        upper+="<svg class='logo' xmlns:dc='http://purl.org/dc/elements/1.1/'xmlns:cc='http://creativecommons.org/ns#'xmlns:rdf='http://www.w3.org/1999/02/22-rdf-syntax-ns#'xmlns:svg='http://www.w3.org/2000/svg'xmlns='http://www.w3.org/2000/svg'viewBox='0 0 716.34668 411.67999'height='411.67999'width='716.34668'xml:space='preserve'id='svg2'version='1.1'><metadata id='metadata8'><rdf:RDF><cc:Work rdf:about=''><dc:format>image/svg+xml</dc:format><dc:type rdf:resource='http://purl.org/dc/dcmitype/StillImage' /></cc:Work></rdf:RDF></metadata><defs id='defs6' /><g transform='matrix(1.3333333,0,0,-1.3333333,0,411.68)'id='g10'><g transform='scale(0.1)'id='g12'><path id='path14'style='fill:#ee252c;fill-opacity:1;fill-rule:nonzero;stroke:none'd='m 1408.52,2023.41 h -67.23 c -101.66,0 -200.05,-67.23 -200.05,-262.35 0,-188.57 90.19,-249.24 200.05,-249.24 h 67.23 z M 1303.58,1406.87 1103.53,957.602 H 957.598 l 219.722,480.438 c -103.3,52.47 -172.17,147.57 -172.17,323.02 0,245.95 155.77,368.93 341.06,368.93 h 188.56 V 957.602 h -126.25 v 449.268 h -104.94' /><path id='path16'style='fill:#040606;fill-opacity:1;fill-rule:nonzero;stroke:none'd='m 3987.78,1370.8 c 0,259.08 100.03,424.69 293.52,424.69 73.78,0 116.41,-19.68 152.49,-42.63 l -18.04,-121.34 c -34.43,29.52 -83.62,57.39 -136.09,57.39 -91.83,0 -157.41,-103.3 -157.41,-311.55 0,-206.6 49.18,-324.66 152.49,-324.66 62.3,0 109.86,24.59 139.37,50.83 l 29.51,-95.1 c -44.27,-37.719 -96.74,-62.309 -172.17,-62.309 -177.09,0 -283.67,142.649 -283.67,424.679 z m -373.85,-1.64 V 957.602 h -127.9 v 826.418 h 127.9 v -382.06 l 198.4,382.06 h 129.55 L 3736.91,1395.4 3971.39,957.602 h -139.38 z m -601.77,77.07 h 211.52 c 0,141.02 -22.96,244.32 -96.74,244.32 -78.71,0 -106.58,-96.74 -114.78,-244.32 z m 141.01,-500.109 c -170.52,0 -275.47,136.089 -275.47,390.249 0,265.63 73.79,459.12 249.24,459.12 137.74,0 227.92,-103.3 227.92,-383.7 v -70.5 h -344.34 c 0,-186.93 52.47,-285.32 154.13,-285.32 72.15,0 129.54,32.8 162.33,55.76 l 29.52,-98.38 c -52.47,-40.998 -121.34,-67.229 -203.33,-67.229 z m -544.38,734.589 h -142.66 v -14.75 c 0,-173.81 -8.2,-427.97 -77.06,-603.42 h 219.72 z m 191.84,-885.448 h -114.78 v 162.34 h -354.18 v -162.34 h -114.78 v 267.278 h 50.84 c 75.42,175.45 83.62,431.25 83.62,650.97 v 70.51 h 383.7 v -721.48 h 65.58 z M 2143.11,1784.02 V 957.602 H 2016.85 V 1328.17 H 1826.64 V 957.602 h -127.89 v 826.418 h 127.89 v -350.9 h 190.21 v 350.9 h 126.26' /></g></g></svg><form method='GET' action='http://www.yandex.ru/'> <input type='hidden' name='clid' value='2308389' /> <input type='text' class='search-box' name='q' autofocus autocomplete='on' placeholder='Search the web with Yandex' /> </form> </div> <div style='text-align: center;width: 80%;margin: auto; margin-top: 50px'>";
    } else {
        upper+="<form method='GET' action='"+srchstr+"'> <input type='text' class='search-box' name='q' autofocus autocomplete='on' placeholder='Search the web ' /> </form> </div> <div style='text-align: center;width: 80%;margin: auto; margin-top: 50px'>";
    }
    QString lower="<img src='img/add.png' class='add-box' onclick='getNewBoxUrl()'></img> </div> <script> function getNewBoxUrl(){prompt('ifRiVOjEJzLc2MQ9h3xT')} </script> </body> </html>";
    QString middle="";
    QFile inputFile(QDir::homePath()+"/.crusta_db/startpage.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       in.setCodec("UTF-8");
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(!(data[0]=="" || data[1]=="")){
              QFile imf(QDir::homePath()+"/.crusta_db/speeddial/img/"+data[0]+".png");
              QString s;
              if(imf.exists())
                  s="<a href='"+data[1]+"'> <div class='box'> <div class='box-image' style='background-image: url(\"img/" + data[0] + ".png\")'> <button class='box-btn' onclick='confirm(\"ifRiVOjEJzLc2MQ9h3xT "+data[0]+"\"); return false;'>&#x2715;</button> </div> <div class='box-title'>"+data[0]+"</div> </div> </a>";
              else
                  s="<a href='"+data[1]+"'> <div class='box'> <div class='box-image' style='background-image: url(\"img/" + data[0] + ".png\")'> <button class='box-btn' onclick='confirm(\"ifRiVOjEJzLc2MQ9h3xT "+data[0]+"\"); return false;'>&#x2715;</button> </div> <div class='box-title'>"+data[0]+"</div> </div> </a>";
              middle+=s;
          }
       }
       inputFile.close();
    }
    QFile f(QDir::homePath()+"/.crusta_db"+"/speeddial/index.html");
    if(f.open(QIODevice::WriteOnly))
    {
        QString s=upper+middle+lower;
        QTextStream t(&f);
        t.setCodec("UTF-8");
        f.resize(0);
        t << s;
        f.close();
    }
}

void SpeedDial::add(QString title,QString url){
    if(!(url.startsWith("http://")||url.startsWith("https://")))
        url="http://"+url;
    if(title.split(" ").length()>1)
        title=title.split(" ")[0];
    QFile f(QDir::homePath()+"/.crusta_db/startpage.txt");
    if(f.open(QIODevice::Append))
    {
        QTextStream t(&f);
        t.setCodec("UTF-8");
        t << title+">>>>>"+url+"\n";
        f.close();
    }
    save(QSettings("Tarptaeya", "Crusta").value("speeddial_bgimage").toString(),QSettings("Tarptaeya", "Crusta").value("speeddial_srch_engine").toString());
}

void SpeedDial::remove(QString forbidden){
    QDir img;
    img.remove(QDir::homePath()+"/.crusta_db"+"/speeddial/img/"+forbidden+".png");
    QFile f(QDir::homePath()+"/.crusta_db/startpage.txt");
    if(f.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QString s;
        QTextStream t(&f);
        t.setCodec("UTF-8");
        while(!t.atEnd())
        {
            QString line = t.readLine();
            if(line.split(">>>>>")[0]!=forbidden)
                s.append(line + "\n");
        }
        f.resize(0);
        t << s;
        f.close();
    }
    save(QSettings("Tarptaeya", "Crusta").value("speeddial_bgimage").toString(),QSettings("Tarptaeya", "Crusta").value("speeddial_srch_engine").toString());
}

void SpeedDial::configure(){
    QString a=QCoreApplication::applicationDirPath()+"/speeddial/img/";
    QString b=QDir::homePath()+"/.crusta_db/speeddial/img/";
    QDir d(QCoreApplication::applicationDirPath()+"/speeddial/img/");
    QStringList filesList = d.entryList(QStringList("*"));
    for(QString file:filesList){
        QFile::copy(a+file,b+file);
    }
}
