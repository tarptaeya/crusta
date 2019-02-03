#include "common-defs.h"
#include "database.h"
#include "mainapplication.h"

#include <QDebug>
#include <QSqlError>

DataBase::DataBase(QObject *parent)
    : QObject (parent)
{
    m_db = QSqlDatabase::addDatabase(QSL("QSQLITE"));
    m_name = QSL("data");
    m_db.setDatabaseName(m_name);
    if (!m_db.open()) {
        qWarning() << m_db.lastError();
        return;
    }
    createTables();
}

DataBase::~DataBase()
{
    m_db.close();
    QSqlDatabase::removeDatabase(m_name);
}

void DataBase::createTables()
{
    QSqlQuery query;
    query.exec(QSL("CREATE TABLE IF NOT EXISTS history (lastVisited DATETIME, title TEXT, url TEXT, profile TEXT, PRIMARY KEY (url, profile))"));
    query.exec(QSL("CREATE TABLE IF NOT EXISTS bookmarks (title TEXT, url TEXT, folder TEXT, profile TEXT, PRIMARY KEY (url, profile))"));
}

void DataBase::addHistory(const HistoryItem &item)
{
    if (appManager->isPrivate()) {
        return;
    }

    QSqlQuery query(QSL("INSERT OR REPLACE INTO history VALUES (?, ?, ?, ?)"));
    query.addBindValue(item.dateTime.toTime_t());
    query.addBindValue(item.title);
    query.addBindValue(item.url);
    query.addBindValue(appManager->webEngineProfile()->storageName());
    query.exec();
}

void DataBase::removeHistory(const QString &address)
{
    QSqlQuery query(QSL("DELETE FROM history WHERE url = ? AND profile = ?"));
    query.addBindValue(address);
    query.addBindValue(appManager->webEngineProfile()->storageName());
    query.exec();
}

QList<HistoryItem> DataBase::history() const
{
    QList<HistoryItem> entries;
    QSqlQuery query(QSL("SELECT * FROM history WHERE profile = ?"));
    query.addBindValue(appManager->webEngineProfile()->storageName());
    query.exec();
    while (query.next()) {
        HistoryItem item;
        item.dateTime = QDateTime::fromTime_t(query.value(0).toUInt());
        item.title = query.value(1).toString();
        item.url = query.value(2).toString();
        entries.append(item);
    }

    return entries;
}

void DataBase::addBookmark(const BookmarkItem &item)
{
    QSqlQuery query(QSL("INSERT INTO bookmarks VALUES (?, ?, ?, ?)"));
    query.addBindValue(item.title);
    query.addBindValue(item.address);
    query.addBindValue(item.folder);
    query.addBindValue(appManager->webEngineProfile()->storageName());
    query.exec();
}

void DataBase::removeBookmark(const QString &address)
{
    QSqlQuery query(QSL("DELETE FROM bookmarks WHERE url = ? AND profile = ?"));
    query.addBindValue(address);
    query.addBindValue(appManager->webEngineProfile()->storageName());
    query.exec();
}

void DataBase::updateBookmark(const BookmarkItem &item)
{
    QSqlQuery query(QSL("UPDATE bookmarks SET title = ?, folder = ? WHERE url = ? AND profile = ?"));
    query.addBindValue(item.title);
    query.addBindValue(item.folder);
    query.addBindValue(item.address);
    query.addBindValue(appManager->webEngineProfile()->storageName());
    query.exec();
}

QList<BookmarkItem> DataBase::bookmarks(const QString &folderName) const
{
    QList<BookmarkItem> entries;
    QSqlQuery query(QSL("SELECT * FROM bookmarks WHERE folder = ? AND profile = ?"));
    query.addBindValue(folderName);
    query.addBindValue(appManager->webEngineProfile()->storageName());
    query.exec();
    while (query.next()) {
        BookmarkItem item;
        item.title = query.value(0).toString();
        item.address = query.value(1).toString();
        entries.append(item);
    }

    return entries;
}

QStringList DataBase::bookmarkFolders() const
{
    QStringList folders;
    QSqlQuery query(QSL("SELECT DISTINCT folder FROM bookmarks WHERE profile = ?"));
    query.addBindValue(appManager->webEngineProfile()->storageName());
    query.exec();
    while (query.next()) {
        folders << query.value(0).toString();
    }
    return folders;
}

BookmarkItem DataBase::isBookmarked(const QString &address) const
{
    BookmarkItem item;
    QSqlQuery query(QSL("SELECT * FROM bookmarks WHERE url = ? AND profile = ?"));
    query.addBindValue(address);
    query.addBindValue(appManager->webEngineProfile()->storageName());
    query.exec();
    while (query.next()) {
        item.title = query.value(0).toString();
        item.address = query.value(1).toString();
        item.folder = query.value(2).toString();
    }
    return item;
}
