#include "database.h"
#include "../data/historyitem.h"
#include "../data/speeddialitem.h"
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

void Database::createHistoryDatabase()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS history (timestamp INTEGER, favicon BLOB, title TEXT, url TEXT, visitCount INTEGER, loadingTime INTEGER)");
    query.exec();
}

void Database::createSpeeddialDatabase()
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS speeddial (image BLOB, title TEXT, url TEXT)");
    query.exec();
}
