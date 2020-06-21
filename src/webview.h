#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebEngineView>

class WebPage;

class WebView : public QWebEngineView
{
    WebPage *m_webPage = nullptr;
public:
    WebView(QWidget *parent = nullptr);
};

#endif // WEBVIEW_H
