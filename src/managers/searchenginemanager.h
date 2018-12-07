#pragma once

#include <QString>

class SearchEngineManager
{
public:
    explicit SearchEngineManager() { }

    static QString getSearchUrl(const QString &query);
};
