#include "browser_schemes.h"

#include <QFile>
#include <QWebEngineUrlRequestJob>

BrowserSchemeHandler::BrowserSchemeHandler(QObject *parent)
    : QWebEngineUrlSchemeHandler(parent)
{
}

void BrowserSchemeHandler::requestStarted(QWebEngineUrlRequestJob *job)
{
    const QString host = job->requestUrl().host();
    QFile *file = new QFile(QStringLiteral(":assets/html/%1.html").arg(host));
    if (!file) {
        job->fail(QWebEngineUrlRequestJob::UrlNotFound);
    }

    job->reply(QByteArray("text/html"), file);
    connect(job, &QWebEngineUrlRequestJob::destroyed, file, &QFile::deleteLater);
}
