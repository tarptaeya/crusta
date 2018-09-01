#include "externaljsobject.h"

ExternalJsObject::ExternalJsObject(QObject *parent, QWebEnginePage *page)
    : QObject(parent)
{
    m_speeddial = new Speeddial(this, page);
}

void ExternalJsObject::setUpWebChannel(QWebChannel *channel, QWebEnginePage *page)
{
    ExternalJsObject *externalJsObject = new ExternalJsObject(channel, page);
    channel->registerObject("externalObject", externalJsObject);
}

QObject *ExternalJsObject::speeddial() const
{
    return m_speeddial;
}
