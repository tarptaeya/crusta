#pragma once

#include <QObject>
#include <QSqlDatabase>

class HistoryItem;

class Database : public QObject
{
public:
    explicit Database(QObject *parent = nullptr);
    void createDatabases();

    bool addHistoryEntry(HistoryItem item);
private:
    QSqlDatabase m_db;
    void createHistoryDatabase();
    void createSpeeddialDatabase();
};
