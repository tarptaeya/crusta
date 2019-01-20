#ifndef CR_WEB_VIEW_H
#define CR_WEB_VIEW_H

#include <QWebEngineView>

class WebPage;

class WebView : public QWebEngineView
{
public:
    explicit WebView(QWidget *parent = nullptr);
    ~WebView();

    bool isLoading();
private:
    bool m_isLoading = false;
    WebPage *m_webPage = nullptr;
};

#endif
