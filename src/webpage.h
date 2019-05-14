#pragma once

#include <QWebEnginePage>
#include <QWebEngineProfile>

#include "searchengine.h"

class WebPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit WebPage(QWebEngineProfile *profile, QObject *parent = nullptr);

    static QWidget *featureWidget(QWebEnginePage *page, const QUrl &securityOrigin, QWebEnginePage::Feature feature);

Q_SIGNALS:
    void engineFound(QWidget *widget, Engine engine);
};
