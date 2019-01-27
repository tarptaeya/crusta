#ifndef CR_SCHEME_H
#define CR_SCHEME_H

#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>

class Scheme : public QWebEngineUrlSchemeHandler
{
public:
    explicit Scheme(QObject *parent = nullptr);

    void requestStarted(QWebEngineUrlRequestJob *job);
};

#endif
