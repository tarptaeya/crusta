#include "bookmarks.h"
#include "browser.h"
#include "database.h"

#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <iostream>

Database::Database(bool isPrivate)
{
    const QString driver(QStringLiteral("QSQLITE"));
    if (!QSqlDatabase::isDriverAvailable(driver)) {
        std::cerr << "driver not available: " << driver.toStdString() << std::endl;
        return;
    }

    m_db = QSqlDatabase::addDatabase(driver);

    if (isPrivate) {
        m_db.setDatabaseName(QStringLiteral(""));
    } else {
        const QDir standardLocation(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        const QString dbPath = standardLocation.absoluteFilePath(QStringLiteral("database"));
        m_db.setDatabaseName(dbPath);
    }

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
    const QString limitHistoryQuery(QStringLiteral("delete from history where url not in ("
                                                   "select url from history order by timestamp desc limit %1)"));

    QSqlQuery query;
    if (!query.exec(limitHistoryQuery.arg(limit))) {
        std::cerr << "unable to limit history table: " << query.lastError().text().toStdString() << std::endl;
    }
}

void Database::createTables()
{
    const QString historyTableQuery(QStringLiteral("create table if not exists history ("
                                                   "icon BLOB,"
                                                   "title text,"
                                                   "url text primary key,"
                                                   "timestamp datetime default current_timestamp)"));

    const QString bookmarksTableQuery(QStringLiteral("create table if not exists bookmarks ("
                                                     "icon BLOB,"
                                                     "title text,"
                                                     "url text primary key,"
                                                     "description text,"
                                                     "folder text)"));

    QSqlQuery sql;
    if (!sql.exec(historyTableQuery)) {
        std::cerr << "unable to create history table: "<< sql.lastError().text().toStdString() << std::endl;
    }

    if (!sql.exec(bookmarksTableQuery)) {
        std::cerr << "unable to create bookmarks table: " << sql.lastError().text().toStdString() << std::endl;
    }
}
