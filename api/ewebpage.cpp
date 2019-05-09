#include "eobject.h"
#include "ewebpage.h"
#include "src/utils.h"

#include <QWebChannel>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

EWebPage::EWebPage(QObject *parent)
    : QWebEnginePage (new QWebEngineProfile, parent)
{
    QWebEngineScript webChannelScript;
    webChannelScript.setName(QStringLiteral("webchannel"));
    webChannelScript.setSourceCode(Utils::readFile(QStringLiteral(":/qtwebchannel/qwebchannel.js")));
    webChannelScript.setInjectionPoint(QWebEngineScript::DocumentCreation);
    webChannelScript.setWorldId(QWebEngineScript::ApplicationWorld);
    webChannelScript.setRunsOnSubFrames(false);
    profile()->scripts()->insert(webChannelScript);

    QWebEngineScript eobjectScript;
    eobjectScript.setName(QStringLiteral("eobject"));
    eobjectScript.setSourceCode(Utils::readFile(QStringLiteral(":/api/eobject.js")));
    eobjectScript.setInjectionPoint(QWebEngineScript::DocumentReady);
    eobjectScript.setWorldId(QWebEngineScript::ApplicationWorld);
    eobjectScript.setRunsOnSubFrames(false);
    profile()->scripts()->insert(eobjectScript);

    EObject *eobject = new EObject(this);
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("browser"), eobject);
    setWebChannel(channel, QWebEngineScript::ApplicationWorld);

    load(QUrl("http://example.com/"));

    profile()->setParent(this);
}

EWebPage::~EWebPage()
{
    webChannel()->deleteLater();
}
