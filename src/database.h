#pragma once

#include <QSqlDatabase>

class Database
{
public:
    explicit Database();
    ~Database();

private:
    QSqlDatabase m_db;

    void createTables();
};
