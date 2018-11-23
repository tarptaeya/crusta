#include "crustascheme.h"
#include <QBuffer>

CrustaScheme::CrustaScheme(QObject *parent)
    : QWebEngineUrlSchemeHandler(parent)
{
}

void CrustaScheme::requestStarted(QWebEngineUrlRequestJob *job)
{
    QByteArray contentType("text/html");
    QBuffer *buffer = new QBuffer();
    buffer->setData("<h1>Hello World</h1>");
    job->reply(contentType, buffer);
}
