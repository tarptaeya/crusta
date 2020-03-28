#pragma once

#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <QWebEngineUrlRequestInterceptor>

class WebPage;

class WebView : public QWebEngineView
{
    WebPage *m_webpage = nullptr;
    void show_context_menu(const QPoint &pos);
    void save_history_entry();
public:
    explicit WebView(QWidget *parent = nullptr);
    void home();

    QWebEngineView * createWindow(QWebEnginePage::WebWindowType type) override;
};

class WebPage : public QWebEnginePage
{
public:
    explicit WebPage(QWebEngineProfile *profile, QObject *parent);
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) override;
};
