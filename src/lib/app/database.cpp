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
    query.exec(QSL("CREATE TABLE IF NOT EXISTS history (iconUrl TEXT, title TEXT, url TEXT PRIMARY KEY, lastVisited DATETIME)"));
}

void DataBase::addHistory(const QWebEngineHistoryItem &item)
{
    QSqlQuery query(QSL("INSERT OR REPLACE INTO history VALUES (?, ?, ?, ?)"));
    query.addBindValue(item.iconUrl());
    query.addBindValue(item.title());
    query.addBindValue(item.url().toString(QUrl::RemoveQuery));
    query.addBindValue(item.lastVisited().toTime_t());
    query.exec();
}
