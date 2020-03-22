#pragma once

#include <QLineEdit>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>

class WebView;

class Tab : public QWidget
{
    QToolBar *m_toolbar = nullptr;
    WebView *m_webview = nullptr;

    QToolButton *m_back_button = nullptr;
    QToolButton *m_forward_button = nullptr;
    QToolButton *m_refresh_button = nullptr;
    QToolButton *m_home_button = nullptr;

    QLineEdit *m_address_bar = nullptr;

    void setup_toolbar();
public:
    explicit Tab(QWidget *parent = nullptr);
    WebView *webview() const;
};
