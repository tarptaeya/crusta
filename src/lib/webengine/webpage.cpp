#include "common-defs.h"
#include "jsobject.h"
#include "mainapplication.h"
#include "notifier.h"
#include "plugininterface.h"
#include "plugins.h"
#include "webpage.h"

#include <QWebChannel>
#include <QWebEngineScript>
#include <QWebEngineSettings>

WebPage::WebPage(QWebEngineProfile *profile)
    : QWebEnginePage (profile)
{
    QWebChannel *webChannel = new QWebChannel(this);
    JsObject *jsObject = new JsObject(this);
    webChannel->registerObject(QSL("crusta"), jsObject);
    setWebChannel(webChannel, QWebEngineScript::ApplicationWorld);

    loadSettings();

    connect(this, &WebPage::featurePermissionRequested, this, &WebPage::handleFeatureRequest);
}

void WebPage::loadSettings()
{
    settings()->setAttribute(QWebEngineSettings::AutoLoadImages, appManager->settings()->value(QSL("webPage/autoLoadImages"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, appManager->settings()->value(QSL("webPage/javascriptEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, appManager->settings()->value(QSL("webPage/javascriptCanOpenWindows"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, appManager->settings()->value(QSL("webPage/javascriptCanAccessClipboard"), false).toBool());
    settings()->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, appManager->settings()->value(QSL("webPage/linksIncludedInFocusChain"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, appManager->settings()->value(QSL("webPage/localStorageEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, appManager->settings()->value(QSL("webPage/localContentCanAccessRemoteUrls"), false).toBool());
    settings()->setAttribute(QWebEngineSettings::XSSAuditingEnabled, appManager->settings()->value(QSL("webPage/xssAuditingEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::SpatialNavigationEnabled, appManager->settings()->value(QSL("webPage/spatialNavigationEnabled"), false).toBool());
    settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, appManager->settings()->value(QSL("webPage/localContentCanAccessFileUrls"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::HyperlinkAuditingEnabled, appManager->settings()->value(QSL("webPage/hyperLinkAuditingEnabled"), false).toBool());
    settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, appManager->settings()->value(QSL("webPage/scrollAnimatorEnabled"), false).toBool());
    settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, appManager->settings()->value(QSL("webPage/errorPageEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::PluginsEnabled, appManager->settings()->value(QSL("webPage/pluginsEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, appManager->settings()->value(QSL("webPage/fullScreenSupportEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, appManager->settings()->value(QSL("webPage/screenCaptureEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::WebGLEnabled, appManager->settings()->value(QSL("webPage/webGLEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, appManager->settings()->value(QSL("webPage/accelerated2dCanvasEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::AutoLoadIconsForPage, appManager->settings()->value(QSL("webPage/autoLoadIconsForPage"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::TouchIconsEnabled, appManager->settings()->value(QSL("webPage/touchIconsEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::FocusOnNavigationEnabled, appManager->settings()->value(QSL("webPage/focusOnNavigationEnabled"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::PrintElementBackgrounds, appManager->settings()->value(QSL("webPage/printElementBackground"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::AllowGeolocationOnInsecureOrigins, appManager->settings()->value(QSL("webPage/allowGeoLocationOnInsecureOrigins"), false).toBool());
    settings()->setAttribute(QWebEngineSettings::AllowWindowActivationFromJavaScript, appManager->settings()->value(QSL("allowWindowActivationFromJavaScript"), false).toBool());
    settings()->setAttribute(QWebEngineSettings::ShowScrollBars, appManager->settings()->value(QSL("webPage/showScrollBars"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::PlaybackRequiresUserGesture, appManager->settings()->value(QSL("webPage/playbackRequiresUserGesture"), true).toBool());
    settings()->setAttribute(QWebEngineSettings::JavascriptCanPaste, appManager->settings()->value(QSL("webPage/javascriptCanPaste"), false).toBool());
    settings()->setAttribute(QWebEngineSettings::WebRTCPublicInterfacesOnly, appManager->settings()->value(QSL("webPage/webRTCPublicInterfacesOnly"), false).toBool());
    settings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, appManager->settings()->value(QSL("webPage/dnsPrefetchEnabled"), true).toBool());
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

void WebPage::handleFeatureRequest(const QUrl &securityOrigin, QWebEnginePage::Feature feature)
{
    Notifier notifier;
    notifier.setData(securityOrigin, feature);
    if (notifier.exec() != Notifier::Accepted) {
        setFeaturePermission(securityOrigin, feature, WebPage::PermissionDeniedByUser);
        return;
    }

    setFeaturePermission(securityOrigin, feature, WebPage::PermissionGrantedByUser);
}
