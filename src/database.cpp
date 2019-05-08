#include "database.h"

#include <QSqlError>
#include <QSqlQuery>
#include <iostream>

Database::Database()
{
    const QString driver(QStringLiteral("QSQLITE"));
    if (!QSqlDatabase::isDriverAvailable(driver)) {
        std::cerr << "driver not available: " << driver.toStdString() << std::endl;
        return;
    }

    m_db = QSqlDatabase::addDatabase(driver);
    // TODO: change name when incognito
    m_db.setDatabaseName(QStringLiteral("temp-database"));
    if (!m_db.open()) {
        std::cerr << "unable to open database: " << m_db.lastError().text().toStdString() << std::endl;
        return;
    }

    createTables();
}

Database::~Database()
{
    if (m_db.isOpen()) {
        m_db.close();
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
