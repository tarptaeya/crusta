#pragma once

#include <QVBoxLayout>
#include <QWidget>

class WebView;

class Tab : public QWidget
{
public:
    explicit Tab(QWidget *parent = nullptr);

    WebView *webView() const;

    bool isInResponsiveMode();
    void enterResponsiveMode(int width, int height);
    void exitResponsiveMode();

    void resizeEvent(QResizeEvent *resize);

    static void responsiveControllPane(Tab *tab);

private:
    WebView *m_webView = nullptr;
    QVBoxLayout *m_layout = nullptr;

    bool m_isInResponsiveMode = false;
    int m_responsiveWidth = 0;
    int m_responsiveHeight = 0;

    void resizeWebView(int width, int height);
};
