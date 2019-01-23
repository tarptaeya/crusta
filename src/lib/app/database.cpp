#include "common-defs.h"
#include "database.h"

#include <QDebug>
#include <QSqlError>

DataBase::DataBase(QObject *parent)
    : QObject (parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(QSL("QSQLITE"));
    db.setDatabaseName(QSL("data"));
    if (!db.open()) {
        qWarning() << db.lastError();
        return;
    }
    createTables();
}

void DataBase::createTables()
{
    QSqlQuery query;
    query.exec(QSL("CREATE TABLE IF NOT EXISTS history (lastVisited DATETIME, title TEXT, url TEXT PRIMARY KEY)"));
}

void DataBase::addHistory(const HistoryItem &item)
{
    QSqlQuery query(QSL("INSERT OR REPLACE INTO history VALUES (?, ?, ?)"));
    query.addBindValue(item.dateTime.toTime_t());
    query.addBindValue(item.title);
    query.addBindValue(item.url);
    query.exec();
}

void DataBase::removeHistory(const QString &address)
{
    QSqlQuery query(QSL("DELETE FROM history WHERE url = ?"));
    query.addBindValue(address);
    query.exec();
}

QList<HistoryItem> DataBase::history()
{
    QList<HistoryItem> entries;
    QSqlQuery query(QSL("SELECT * FROM history"));
    while (query.next()) {
        HistoryItem item;
        item.dateTime = QDateTime::fromTime_t(query.value(0).toUInt());
        item.title = query.value(1).toString();
        item.url = query.value(2).toString();
        entries.append(item);
    }

    return entries;
}
