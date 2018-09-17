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
#pragma once

#include <QObject>
#include <QWebEnginePage>

class SpeeddialItem;

class Speeddial : public QObject
{
    Q_OBJECT
public:
    explicit Speeddial(QObject *parent, QWebEnginePage *page);
    Q_INVOKABLE void addDial();
    Q_INVOKABLE void removeDial(const QString &url);
    Q_INVOKABLE void loadDialsFromDatabase();
    static QList<SpeeddialItem> speeddialItems();
Q_SIGNALS:
    void dialAdded(QVariantMap map);
    void dialsAdded(QVariantList dials);
private:
    QWebEnginePage *m_page = nullptr;
    static QList<SpeeddialItem> m_speeddialItems;

    void saveDialToDatabase(const QString &title, const QString &url);
};
