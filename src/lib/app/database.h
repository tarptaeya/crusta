#ifndef CR_DATABASE_H
#define CR_DATABASE_H

#include "historyitem.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class DataBase : public QObject
{
public:
    explicit DataBase(QObject *parent = nullptr);

    void createTables();
    void addHistory(const HistoryItem &item);
    void removeHistory(const QString &address);
    QList<HistoryItem> history();
};

#endif
