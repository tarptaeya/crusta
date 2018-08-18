#include "externaljsobject.h"

ExternalJsObject::ExternalJsObject(QObject *parent)
    : QObject(parent)
{
    m_speeddial = new Speeddial(this);
}

QWebChannel *ExternalJsObject::setUpWebChannel()
{
    QWebChannel *channel = new QWebChannel();
    ExternalJsObject *externalJsObject = new ExternalJsObject(channel);
    channel->registerObject("externalObject", externalJsObject);
    return channel;
}

QObject *ExternalJsObject::speeddial() const
{
    return m_speeddial;
}
