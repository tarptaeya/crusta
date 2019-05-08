#pragma once

#include <QObject>
#include <QWebEngineProfile>

class MainWindow;

class Browser : public QObject
{
public:
    explicit Browser(bool isPrivate = false, QObject *parent = nullptr);
    void run();

    void createMainWindow();
    QWebEngineProfile *profile();

    bool isPrivate() const;
    static Browser *instance();

private:
    bool m_isPrivate = false;
    QList<MainWindow *> m_mainWindows;
    QWebEngineProfile *m_profile = nullptr;

    void setupProfile();
};
