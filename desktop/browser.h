#pragma once

#include <QByteArray>
#include <QSqlDatabase>
#include <QWebEngineProfile>

#define browser Browser::instance()

class BrowserWindow;
class PreferencesWindow;

class Browser
{
    bool m_is_private = false;

    QSqlDatabase m_database;
    QWebEngineProfile *m_web_profile = nullptr;
    PreferencesWindow *m_preferences_window = nullptr;

    void setup_database();
    void setup_preferences_window();
    void setup_web_profile();
public:
    ~Browser();
    int start(int argc, char **argv);

    BrowserWindow *create_browser_window() const;
    void register_scheme(const QByteArray &name) const;
    QWebEngineProfile *web_profile() const;

    static Browser *instance();

    void show_preferences_window();
};
