#pragma once

#include <QWebEngineUrlSchemeHandler>

class BrowserSchemeHandler : public QWebEngineUrlSchemeHandler
{
public:
    explicit BrowserSchemeHandler(QObject *parent = nullptr);
    void requestStarted(QWebEngineUrlRequestJob *job) override;
};
