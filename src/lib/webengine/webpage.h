#ifndef CR_WEB_PAGE_H
#define CR_WEB_PAGE_H

#include <QWebEnginePage>
#include <QWebEngineProfile>

class WebPage : public QWebEnginePage
{
public:
    explicit WebPage(QWebEngineProfile *profile);

    void loadSettings();

    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);

private:
    void handleFeatureRequest(const QUrl &securityOrigin, QWebEnginePage::Feature feature);
};

#endif
