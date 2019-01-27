#include "jsobject.h"
#include "searchengine.h"

JsObject::JsObject(QObject *parent)
    : QObject (parent)
{

}

QObject *JsObject::searchEngine()
{
    if (!m_searchEngine) {
        m_searchEngine = new SearchEngine(this);
    }

    return m_searchEngine;
}
