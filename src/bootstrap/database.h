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
#include <QSqlDatabase>

class HistoryItem;
class SpeeddialItem;
class BookmarksItem;

class Database : public QObject
{
public:
    explicit Database(QObject *parent = nullptr);
    void createDatabases();

    bool addHistoryEntry(HistoryItem item);
    bool addSpeeddialEntry(SpeeddialItem item);
    bool removeSpeeddialEntry(const QString &url);
    QList<SpeeddialItem> loadSpeeddialEntries();
    bool addBookmarksEntry(BookmarksItem item);
    BookmarksItem isBookmarked(const QString &urlString);
    bool removeBookmarksEntry(const QString &url);
    bool addCompleterEntry(const QString &entry);
    QStringList loadCompleterEntries();
private:
    QSqlDatabase m_db;
    void createHistoryDatabase();
    void createSpeeddialDatabase();
    void createBookmarksDatabase();
    void createCompleterDatabase();
};
