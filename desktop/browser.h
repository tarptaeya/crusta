#pragma once

#include <QByteArray>
#include <QSqlDatabase>
#include <QWebEngineProfile>

#define browser Browser::instance()

class Adblock;
class BookmarkModel;
class BrowserWindow;
class HistoryModel;

class Browser
{
    bool m_is_private = false;

    QWebEngineProfile *m_web_profile = nullptr;

    QSqlDatabase m_database;
    Adblock *m_adblock = nullptr;
    HistoryModel *m_history_model = nullptr;
    BookmarkModel *m_bookmark_model = nullptr;

    void setup_web_profile();
    void setup_database();
public:
    ~Browser();
    int start(int argc, char **argv);

    BrowserWindow *create_browser_window() const;
    void register_scheme(const QByteArray &name) const;
    QWebEngineProfile *web_profile() const;

    Adblock *adblock() const;
    HistoryModel *history_model() const;
    BookmarkModel *bookmark_model() const;

    static Browser *instance();
};
