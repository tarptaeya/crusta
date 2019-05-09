#pragma once

#include <QWebEnginePage>

class EWebPage : public QWebEnginePage
{
public:
    explicit EWebPage(QObject *parent = nullptr);
    ~EWebPage();
};
