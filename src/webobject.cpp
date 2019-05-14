#include "webobject.h"

#include <QWidget>

WebObject::WebObject(QObject *parent)
    : QObject (parent)
{
    m_searchEngine = new SearchEngine(this);

    connect(m_searchEngine, &SearchEngine::engineFound, this, [this] (QWidget *widget, Engine engine) { emit engineFound(widget, engine); });
}

QObject *WebObject::searchEngine()
{
    return m_searchEngine;
}
