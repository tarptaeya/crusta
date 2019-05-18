#pragma once

#include <QObject>
#include <QWebEngineProfile>

class Database;
class Downloads;
class MainWindow;
class Plugins;
class Tab;

class Browser : public QObject
{
public:
    explicit Browser(bool isPrivate = false, QObject *parent = nullptr);
    ~Browser();
    void run();

    Tab *createMainWindow();
    Downloads *downloads();
    Plugins *plugins();
    QWebEngineProfile *profile();

    bool isPrivate() const;
    static Browser *instance(bool isPrivate = false);

private:
    bool m_isPrivate = false;
    QList<MainWindow *> m_mainWindows;
    QWebEngineProfile *m_profile = nullptr;
    Database *m_database = nullptr;
    Downloads *m_downloads = nullptr;
    Plugins *m_plugins = nullptr;

    void setupProfile();
    void setupWebExtensionPolyfill();
};
