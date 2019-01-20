#include "common-defs.h"
#include "jsobject.h"
#include "mainapplication.h"
#include "plugininterface.h"
#include "plugins.h"
#include "webpage.h"

#include <QWebChannel>
#include <QWebEngineScript>

WebPage::WebPage(QWebEngineProfile *profile)
    : QWebEnginePage (profile)
{
    QWebChannel *webChannel = new QWebChannel(this);
    JsObject *jsObject = new JsObject(this);
    webChannel->registerObject(QSL("crusta"), jsObject);
    setWebChannel(webChannel, QWebEngineScript::ApplicationWorld);
}

bool WebPage::acceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
    for (Plugin *plugin : appManager->plugins()->plugins()) {
        if (!plugin->enabled) {
            continue;
        }

        if (!plugin->interface->callAcceptNavigationRequest(url, type, isMainFrame)) {
            return false;
        }
    }

    return true;
}
