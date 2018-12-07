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
#include "database.h"
#include "historyitem.h"
#include "speeddialitem.h"
#include "bookmarksitem.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Database::Database(QObject *parent)
    : QObject(parent)
{
}

void Database::createDatabases()
{
    m_db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));
    m_db.setDatabaseName(QStringLiteral("crusta.db.sqlite"));
    if (!m_db.open()) {
        qWarning() << "ERROR OPENING DATABASE:" << m_db.lastError();
        return;
    }
    createHistoryDatabase();
    createSpeeddialDatabase();
    createBookmarksDatabase();
    createCompleterDatabase();
}

bool Database::addHistoryEntry(HistoryItem item)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM history WHERE url = ?");
    query.addBindValue(item.url());
    if (query.exec() && query.next()) {
        query.prepare("UPDATE history SET timestamp = ?, visitCount = visitCount + 1, loadingTime = loadingTime + ? WHERE url = ?");
        query.addBindValue(item.timestamp());
        query.addBindValue(item.loadingTime());
        query.addBindValue(item.url());
    } else {
        query.prepare("INSERT INTO history (timestamp, favicon, title, url, visitCount, loadingTime) VALUES (?, ?, ?, ?, 1, ?)");
        query.addBindValue(item.timestamp());
        query.addBindValue(item.favicon());
        query.addBindValue(item.title());
        query.addBindValue(item.url());
        query.addBindValue(item.loadingTime());
    }
    return query.exec();
}

bool Database::addSpeeddialEntry(SpeeddialItem item)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM speeddial WHERE url = ?");
    query.addBindValue(item.url());
    if (query.exec() && query.next()) {
        if (!item.title().isEmpty() && !item.image().isEmpty()) {
            query.prepare("UPDATE speeddial SET image = ?  WHERE url = ?");
            query.addBindValue(item.image());
            query.addBindValue(item.url());
        } else {
            return false;
        }
    } else {
        query.prepare("INSERT INTO speeddial (image, title, url) VALUES (?, ?, ?)");
        query.addBindValue(item.image());
        query.addBindValue(item.title());
        query.addBindValue(item.url());
    }
    return query.exec();
}

bool Database::removeSpeeddialEntry(const QString &url)
{
    QSqlQuery query;
    query.prepare("DELETE FROM speeddial WHERE url = ?");
    query.addBindValue(url);
    return query.exec();
}

QList<SpeeddialItem> Database::loadSpeeddialEntries()
{
    QList<SpeeddialItem> speeddialItems;
    QSqlQuery query;
    query.prepare("SELECT * FROM speeddial");
    query.exec();
    while (query.next()) {
        QByteArray image = query.value(0).toByteArray();
        QString title = query.value(1).toString();
        QString url = query.value(2).toString();
        SpeeddialItem item;
        item.setImage(image);
        item.setTitle(title);
        item.setUrl(url);
        speeddialItems.append(item);
    }
    return speeddialItems;
}

bool Database::addBookmarksEntry(BookmarksItem item)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM bookmarks WHERE url = ?");
    query.addBindValue(item.url());
    if (query.exec() && query.next()) {
        query.prepare("UPDATE bookmarks SET title = ?, folder = ?, favicon = ? WHERE url = ?");
        query.addBindValue(item.title());
        query.addBindValue(item.folder());
        query.addBindValue(item.favicon());
        query.addBindValue(item.url());
    } else {
        query.prepare("INSERT INTO bookmarks (favicon, title, url, folder) VALUES (?, ?, ?, ?)");
        query.addBindValue(item.favicon());
        query.addBindValue(item.title());
        query.addBindValue(item.url());
        query.addBindValue(item.folder());
    }
    return query.exec();

}

BookmarksItem Database::isBookmarked(const QString &urlString)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM bookmarks WHERE url = ?");
    query.addBindValue(urlString);
    if (query.exec() && query.next()) {
        QString title = query.value(1).toString();
        QString url = query.value(2).toString();
        BookmarksItem item;
        item.setTitle(title);
        item.setUrl(url);
        return item;
    }
    return BookmarksItem();
}

bool Database::removeBookmarksEntry(const QString &url)
{
    QSqlQuery query;
    query.prepare("DELETE FROM bookmarks WHERE url = ?");
    query.addBindValue(url);
    return query.exec();
}

bool Database::addCompleterEntry(const QString &entry)
{
    QSqlQuery query;
    query.prepare("INSERT INTO completer VALUES (?)");
    query.addBindValue(entry);
    return query.exec();
}

QStringList Database::loadCompleterEntries()
{
    QStringList entries;
    QSqlQuery query;
    query.prepare("SELECT * FROM completer");
    query.exec();
    while (query.next()) {
        entries.append(query.value(0).toString());
    }
    return entries;
}

void Database::createHistoryDatabase()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS history (timestamp INTEGER, favicon BLOB, title TEXT, url TEXT PRIMARY KEY, visitCount INTEGER, loadingTime INTEGER)");
    query.exec();
}

void Database::createSpeeddialDatabase()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS speeddial (image BLOB, title TEXT, url TEXT PRIMARY KEY)");
    query.exec();
}

void Database::createBookmarksDatabase()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS bookmarks (favicon BLOB, title TEXT, url TEXT PRIMARY KEY, folder TEXT)");
    query.exec();
}

void Database::createCompleterDatabase()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS completer (entry TEXT UNIQUE)");
    query.exec();
}
