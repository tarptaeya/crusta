#pragma once

#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>

class CRScheme : public QWebEngineUrlSchemeHandler
{
public:
    CRScheme(QObject *parent = nullptr);

    void requestStarted(QWebEngineUrlRequestJob *request);
};
