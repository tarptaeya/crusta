#include "webobject.h"

#include <QWidget>

WebObject::WebObject(QObject *parent)
    : QObject (parent)
{
    m_searchEngine = new SearchEngine(this);
    m_startPage = new StartPage(this);

    connect(m_searchEngine, &SearchEngine::engineFound, [this] (QWidget *widget) { emit popupRequested(widget); });
}

QObject *WebObject::searchEngine()
{
    return m_searchEngine;
}

QObject *WebObject::startPage()
{
    return m_startPage;
}
