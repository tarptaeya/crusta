#pragma once

#include <QWebEnginePage>
#include <QWebEngineProfile>

class WebPage : public QWebEnginePage
{
    Q_OBJECT
public:
    explicit WebPage(QWebEngineProfile *profile, QObject *parent = nullptr);

    static QWidget *featureWidget(QWebEnginePage *page, const QUrl &securityOrigin, QWebEnginePage::Feature feature);

Q_SIGNALS:
    void popupRequested(QWidget *widget);
};
