#pragma once

#include <QString>
#include <QIcon>

class SearchEngine;

class SearchEngineManager
{
public:
    explicit SearchEngineManager() { }

    static void addSearchEngine(const SearchEngine &engine);
    static void removeSearchEngine(const SearchEngine &engine);
    static QString getSearchUrl(const QString &query);

    static QList<SearchEngine> s_engines;
private:
    static QString s_currentUrl;
};

class SearchEngine
{
public:
    QIcon icon;
    QString name;
    QString url;
    QString description;
};
