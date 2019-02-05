#include "common-defs.h"
#include "mainapplication.h"
#include "searchenginemanager.h"

#include <QLocale>

SearchEngineManager::SearchEngineManager(QWidget *parent)
    : QWidget (parent)
{
    if (appManager->settings()->value(QSL("searchEngine/engines")).toString().isEmpty()) {
        QStringList engines;
        const QStringList ecosia(QStringList() << QSL("Ecosia")
                                 << QSL("https://www.ecosia.org/search?tt=crusta&q={searchTerms}")
                                 << QSL("https://ac.ecosia.org/?q={searchTerms}"));
        const QStringList yandex(QStringList() << QSL("Yandex")
                                 << QSL("http://www.yandex.ru/?clid=2308389&text={searchTerms}")
                                 << QSL(""));

        engines << ecosia << yandex;
        appManager->settings()->setValue(QSL("searchEngine/engines"), engines);

        if (QLocale::system().language() == QLocale::Russian) {
            appManager->settings()->setValue(QSL("searchEngine/defaultEngine"), yandex);
        } else {
            appManager->settings()->setValue(QSL("searchEngine/defaultEngine"), ecosia);
        }
    }

    m_engines = appManager->settings()->value(QSL("searchEngine/engines")).toStringList();
}

QStringList SearchEngineManager::engines() const
{
    return m_engines;
}

void SearchEngineManager::addSearchEngine(const QStringList &engine)
{
    m_engines.append(engine);
    appManager->settings()->setValue(QSL("searchEngine/engines"), m_engines);
}

QString SearchEngineManager::getQuery(const QString &query) const
{
    QString queryUrl = appManager->settings()->value(QSL("searchEngine/defaultEngine")).toStringList()[1];
    queryUrl = queryUrl.replace(QSL("{searchTerms}"), query);
    return queryUrl;
}
