#include "crustascheme.h"
#include "tools.h"
#include <QBuffer>

CrustaScheme::CrustaScheme(QObject *parent)
    : QWebEngineUrlSchemeHandler(parent)
{
}

void CrustaScheme::requestStarted(QWebEngineUrlRequestJob *job)
{
    QString data;
    if (job->requestUrl().toString() == QStringLiteral("crusta:home")) {
        data = readFile(":/html/home.html");
    } else if (job->requestUrl().toString() == QStringLiteral("crusta:about")) {
        data = readFile(":/html/about.html");
    } else if (job->requestUrl().toString() == QStringLiteral("crusta:feedback")) {
        data = readFile(":/html/feedback.html");
    }
    QByteArray contentType("text/html");
    QBuffer *buffer = new QBuffer();
    buffer->setData(data.toUtf8());
    job->reply(contentType, buffer);
}
