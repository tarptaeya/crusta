#pragma once

#include <QSqlDatabase>

class Database
{
public:
    explicit Database(bool isPrivate);
    ~Database();

    void limitDatabase();

private:
    QSqlDatabase m_db;

    void createTables();
};
