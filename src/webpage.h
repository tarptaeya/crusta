#pragma once

#include <QWebEnginePage>
#include <QWebEngineProfile>

class WebPage : public QWebEnginePage
{
public:
    explicit WebPage(QWebEngineProfile *profile, QObject *parent = nullptr);

    static QWidget *featureWidget(QWebEnginePage *page, const QUrl &securityOrigin, QWebEnginePage::Feature feature);
};
