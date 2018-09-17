/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <tarptaeya@gmail.com>
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
#include "scripts.h"

#include <QFile>
#include <QIODevice>
#include <QDebug>

Scripts::Scripts()
{
}

QString Scripts::webChannelScript()
{
    QFile file1(":qtwebchannel/qwebchannel.js");
    if (!file1.open(QIODevice::ReadOnly)) {
        qDebug() << "qwebchannel.js not found";
        return QStringLiteral("");
    }

    QFile file2(":html/webchannel.js");
    if (!file2.open(QIODevice::ReadOnly)) {
        qDebug() << "webchannel.js not found";
        return QStringLiteral("");
    }
    return QString("%1; %2").arg(QString::fromLatin1(file1.readAll()), QString::fromLatin1(file2.readAll()));
}

QString Scripts::speeddialScript()
{
    QFile file(":html/speeddial/index.js");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "speeddial/index.js not found";
        return QStringLiteral("");
    }

    return QString("%1").arg(QString::fromLatin1(file.readAll()));
}
