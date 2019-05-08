#pragma once

#include <QWebEnginePage>
#include <QWebEngineProfile>

class WebPage : public QWebEnginePage
{
public:
    explicit WebPage(QWebEngineProfile *profile, QObject *parent = nullptr);
};
