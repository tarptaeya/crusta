#pragma once

#include <QVBoxLayout>
#include <QWidget>

class WebView;

class Tab : public QWidget
{
public:
    explicit Tab(QWidget *parent = nullptr);

private:
    WebView *m_webView = nullptr;
    QVBoxLayout *m_layout = nullptr;
};
