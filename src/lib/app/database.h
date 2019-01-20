#ifndef CR_DATABASE_H
#define CR_DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QWebEngineHistoryItem>

class DataBase : public QObject
{
public:
    explicit DataBase(QObject *parent = nullptr);

    void createTables();
    void addHistory(const QWebEngineHistoryItem &item);
};

#endif
