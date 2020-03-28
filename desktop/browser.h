#pragma once

#include <QByteArray>
#include <QSqlDatabase>
#include <QWebEngineProfile>

#define browser Browser::instance()

class Adblock;
class BookmarkModel;
class BrowserWindow;
class HistoryModel;
class Plugins;

class Browser
{
    bool m_is_private = false;

    QWebEngineProfile *m_web_profile = nullptr;

    QSqlDatabase m_database;
    Adblock *m_adblock = nullptr;
    HistoryModel *m_history_model = nullptr;
    BookmarkModel *m_bookmark_model = nullptr;
    Plugins *m_plugins = nullptr;

    void setup_web_profile();
    void setup_database();
    void load_settings();
public:
    ~Browser();
    int start(int argc, char **argv);

    BrowserWindow *create_browser_window() const;
    void register_scheme(const QByteArray &name) const;
    QWebEngineProfile *web_profile() const;

    bool is_private() const;

    Adblock *adblock() const;
    HistoryModel *history_model() const;
    BookmarkModel *bookmark_model() const;
    Plugins *plugins() const;

    static Browser *instance();
};
