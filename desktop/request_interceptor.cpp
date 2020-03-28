#include "request_interceptor.h"

RequestInterceptor::RequestInterceptor(QObject *parent)
    : QWebEngineUrlRequestInterceptor(parent)
{
}

void RequestInterceptor::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    if (m_settings.value(QStringLiteral("privacy/dnt"), true).toBool())
        info.setHttpHeader("DNT", "1");
}
