#include "webpage.h"
#include "../pages/externaljsobject.h"
#include <QWebEngineScript>

WebPage::WebPage(QWebEngineProfile *profile)
    : QWebEnginePage(profile)
{
    QWebChannel *channel = new QWebChannel(this);
    ExternalJsObject::setUpWebChannel(channel, this);
    setWebChannel(channel, QWebEngineScript::ApplicationWorld);
}

WebPage::~WebPage()
{
}
