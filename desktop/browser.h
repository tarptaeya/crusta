#pragma once

#include <QByteArray>
#include <QSqlDatabase>
#include <QWebEngineProfile>

#define browser Browser::instance()

class BrowserWindow;
class HistoryModel;

class Browser
{
    bool m_is_private = false;

    QWebEngineProfile *m_web_profile = nullptr;

    QSqlDatabase m_database;
    HistoryModel *m_history_model = nullptr;

    void setup_web_profile();
    void setup_database();
public:
    ~Browser();
    int start(int argc, char **argv);

    BrowserWindow *create_browser_window() const;
    void register_scheme(const QByteArray &name) const;
    QWebEngineProfile *web_profile() const;

    HistoryModel *history_model() const;

    static Browser *instance();
};
