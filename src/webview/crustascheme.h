#pragma once

#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineUrlRequestJob>

class CrustaScheme : public QWebEngineUrlSchemeHandler
{
public:
    explicit CrustaScheme(QObject *parent = nullptr);
protected:
    void requestStarted(QWebEngineUrlRequestJob *job);
};
