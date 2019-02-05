#include "jsobject.h"
#include "searchengine.h"

JsObject::JsObject(WebPage *page, QObject *parent)
    : QObject (parent)
    , m_page(page)
{

}

QObject *JsObject::searchEngine()
{
    if (!m_searchEngine) {
        m_searchEngine = new SearchEngine(m_page, this);
    }

    return m_searchEngine;
}
