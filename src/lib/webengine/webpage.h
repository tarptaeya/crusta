#ifndef CR_WEB_PAGE_H
#define CR_WEB_PAGE_H

#include <QWebEnginePage>
#include <QWebEngineProfile>

class WebPage : public QWebEnginePage
{
public:
    explicit WebPage(QWebEngineProfile *profile);

    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);
};

#endif
