#pragma once

#include <QObject>
#include <QSqlDatabase>

class HistoryItem;
class SpeeddialItem;

class Database : public QObject
{
public:
    explicit Database(QObject *parent = nullptr);
    void createDatabases();

    bool addHistoryEntry(HistoryItem item);
    bool addSpeeddialEntry(SpeeddialItem item);
    QList<SpeeddialItem> loadSpeeddialEntries();
private:
    QSqlDatabase m_db;
    void createHistoryDatabase();
    void createSpeeddialDatabase();
};
