#pragma once

#include <QWebEngineView>

class WebPage;

class WebView : public QWebEngineView
{
public:
    explicit WebView(QWidget *parent = nullptr);
    void loadStartupUrl();
private:
    WebPage *m_webPage = nullptr;
};
