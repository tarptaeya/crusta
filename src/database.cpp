#include "browser.h"
#include "database.h"

#include <QSqlError>
#include <QSqlQuery>
#include <iostream>

Database::Database(bool isPrivate)
{
    const QString driver(QStringLiteral("QSQLITE"));
    if (!QSqlDatabase::isDriverAvailable(driver)) {
        std::cerr << "driver not available: " << driver.toStdString() << std::endl;
        return;
    }

    m_db = QSqlDatabase::addDatabase(driver);
    m_db.setDatabaseName(isPrivate ? QStringLiteral(":memory:") : QStringLiteral("temp-database"));
    if (!m_db.open()) {
        std::cerr << "unable to open database: " << m_db.lastError().text().toStdString() << std::endl;
        return;
    }

    createTables();
    limitDatabase();
}

Database::~Database()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void Database::limitDatabase()
{
    const int limit = 1000;
    const QString limitQuery(QStringLiteral("delete from %1 where id not in (select id from %1 order by timestamp desc limit %2)"));

    QSqlQuery query;
    if (!query.exec(limitQuery.arg(QStringLiteral("history")).arg(limit))) {
        std::cerr << "unable to limit database: " << query.lastError().text().toStdString() << std::endl;
    }
}

void Database::createTables()
{
    const QString historyTableQuery(QStringLiteral("create table if not exists history (id integer primary key autoincrement,"
                                                   "title text not null,"
                                                   "url text not null,"
                                                   "timestamp datetime default current_timestamp)"));

    QSqlQuery qsql;
    if (!qsql.exec(historyTableQuery)) {
        std::cerr << "unable to execute: " << historyTableQuery.toStdString() << std::endl;
        std::cerr << qsql.lastError().text().toStdString() << std::endl;
    }
}
