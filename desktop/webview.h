#pragma once

#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineView>

class WebPage;

class WebView : public QWebEngineView
{
    WebPage *m_webpage = nullptr;
public:
    explicit WebView(QWidget *parent = nullptr);
};

class WebPage : public QWebEnginePage
{
public:
    explicit WebPage(QWebEngineProfile *profile, QObject *parent);
};
