#include "searchenginemanager.h"
#include "appmanager.h"
#include "database.h"

QList<SearchEngine> SearchEngineManager::s_engines;
QString SearchEngineManager::s_currentUrl;

void SearchEngineManager::addSearchEngine(const SearchEngine &engine)
{
    appManager->database()->addSearchEngine(engine);
    appManager->database()->loadSearchEngines();
}

void SearchEngineManager::removeSearchEngine(const SearchEngine &engine)
{
    appManager->database()->removeSearchEngine(engine.url);
    appManager->database()->loadSearchEngines();
}

QString SearchEngineManager::getSearchUrl(const QString &query)
{
    //return s_currentUrl + query;
    return QString("https://www.ecosia.org/search?tt=crusta&q=") + query;
}
