#include "searchenginemanager.h"

QString SearchEngineManager::getSearchUrl(const QString &query)
{
    return QString("https://www.ecosia.org/search?tt=crusta&q=") + query;
}
