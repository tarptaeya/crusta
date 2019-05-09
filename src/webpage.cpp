#include "api/eobject.h"
#include "webpage.h"

#include <QWebChannel>
#include <QWebEngineScript>

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage (profile, parent)
{
    EObject *eobject = new EObject;
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("browser"), eobject);
    setWebChannel(channel, QWebEngineScript::ApplicationWorld);
}
