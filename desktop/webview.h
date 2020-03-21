#pragma once

#include <QWebEngineView>

class WebView : public QWebEngineView
{
public:
    explicit WebView(QWidget *parent = nullptr);
};
