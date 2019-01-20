#include "common-defs.h"
#include "scheme.h"

#include <QBuffer>
#include <QByteArray>

Scheme::Scheme(QObject *parent)
    : QWebEngineUrlSchemeHandler (parent)
{

}

void Scheme::requestStarted(QWebEngineUrlRequestJob *job)
{
    QString host = job->requestUrl().host();
    QString data = readFile((host.prepend(QSL(":/html/")).append(QSL(".html"))));
    if (data.isEmpty()) {
        data = readFile(QSL(":html/fallback.html"));
    }
    QByteArray contentType("text/html");
    QBuffer *buffer = new QBuffer(this);
    buffer->setData(data.toUtf8());
    job->reply(contentType, buffer);
}
