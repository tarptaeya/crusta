#pragma once

#include <QWidget>

class WebView;

class Tab : public QWidget
{
    WebView *m_webview = nullptr;
public:
    explicit Tab(QWidget *parent = nullptr);
};
