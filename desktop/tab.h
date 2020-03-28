#pragma once

#include <QLineEdit>
#include <QSettings>
#include <QStackedWidget>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>

class WebView;

class Tab : public QWidget
{
    Q_OBJECT
public:
    explicit Tab(QWidget *parent = nullptr);

Q_SIGNALS:
    void title_changed(const QString &title);
    void icon_changed(const QIcon &icon);
};

class WebTab : public Tab
{
public:
    QToolBar *m_toolbar = nullptr;
    WebView *m_webview = nullptr;

    QToolButton *m_back_button = nullptr;
    QToolButton *m_forward_button = nullptr;
    QToolButton *m_refresh_button = nullptr;
    QToolButton *m_home_button = nullptr;

    QLineEdit *m_address_bar = nullptr;
    QAction *m_bookmark_action = nullptr;

    void setup_toolbar();
public:
    explicit WebTab(QWidget *parent = nullptr);
    QToolBar *toolbar() const;
    QLineEdit *address_bar() const;
    WebView *webview() const;
};

class ManagerTab : public Tab
{
    QSettings m_settings;

    QToolBar *m_toolbar = nullptr;
    QStackedWidget *m_stacked_widget = nullptr;

    void setup_toolbar();
    void setup_stacked_widget();
    void setup_settings_widget();
public:
    explicit ManagerTab(QWidget *parent = nullptr);

    void open_settings();
    void open_history();
    void open_bookmarks();
};
