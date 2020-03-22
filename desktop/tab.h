#pragma once

#include <QLineEdit>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>

class WebView;

class Tab : public QWidget
{
    Q_OBJECT

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
    QToolBar *toolbar() const;
    WebView *webview() const;

Q_SIGNALS:
    void title_changed(const QString &title);
    void icon_changed(const QIcon &icon);
};
