#pragma once

#include <QObject>
#include <QWebEngineProfile>

class Database;
class History;
class MainWindow;

class Browser : public QObject
{
public:
    explicit Browser(bool isPrivate = false, QObject *parent = nullptr);
    ~Browser();
    void run();

    void createMainWindow();
    History *history();
    QWebEngineProfile *profile();

    bool isPrivate() const;
    static Browser *instance();

private:
    bool m_isPrivate = false;
    QList<MainWindow *> m_mainWindows;
    QWebEngineProfile *m_profile = nullptr;
    Database *m_database = nullptr;
    History *m_history = nullptr;

    void setupProfile();
    void setupWebExtensionPolyfill();
};
