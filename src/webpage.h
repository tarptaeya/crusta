#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebEnginePage>
#include <QWebEngineProfile>

class WebPage : public QWebEnginePage
{
public:
    WebPage(QWebEngineProfile *profile, QObject *parent);
};

#endif // WEBPAGE_H
