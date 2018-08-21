#pragma once

#include <QWebEngineView>

class WebPage;
class TabWidget;

class WebView : public QWebEngineView
{
public:
    explicit WebView(QWidget *parent = nullptr);
    ~WebView();
    void loadStartupUrl();
    void setVirtualTabWidget(TabWidget *tabWidget);
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
private:
    WebPage *m_webPage = nullptr;
    TabWidget *m_tabWidget = nullptr;
};
