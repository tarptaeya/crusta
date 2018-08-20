#include "webpage.h"
#include "../pages/externaljsobject.h"

WebPage::WebPage(QWebEngineProfile *profile)
    : QWebEnginePage(profile)
{
    QWebChannel *channel = ExternalJsObject::setUpWebChannel();
    setWebChannel(channel);
}

WebPage::~WebPage()
{
}
