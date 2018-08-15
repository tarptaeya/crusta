#pragma once

#include <QWebEnginePage>

class WebPage : public QWebEnginePage
{
public:
    explicit WebPage(QObject *parent = nullptr);
};
