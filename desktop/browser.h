#pragma once

#include <QByteArray>
#include <QWebEngineProfile>

#define browser Browser::instance()

class BrowserWindow;

class Browser
{
    QWebEngineProfile *m_web_profile = nullptr;
    void setup_web_profile();
public:
    int start(int argc, char **argv);

    BrowserWindow *create_browser_window() const;
    void register_scheme(const QByteArray &name) const;
    QWebEngineProfile *web_profile() const;

    static Browser *instance();
};
