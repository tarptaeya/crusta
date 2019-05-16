#include "crscheme.h"

#include <QFile>

CRScheme::CRScheme(QObject *parent)
    : QWebEngineUrlSchemeHandler (parent)
{
}

void CRScheme::requestStarted(QWebEngineUrlRequestJob *request)
{
    QString requestUrl = request->requestUrl().toDisplayString();
    QFile *file = nullptr;
    if (requestUrl == QStringLiteral("crusta:startpage")) {
        file = new QFile(":/html/startpage.html");
        request->reply("text/html", file);
    }

    if (!file) {
        request->fail(QWebEngineUrlRequestJob::UrlNotFound);
        return;
    }

    connect(request, &QWebEngineUrlRequestJob::destroyed, file, &QFile::deleteLater);
}
