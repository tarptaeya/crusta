#pragma once

#include <QObject>
#include <QWebEngineProfile>

class MainWindow;

class Browser : public QObject
{
public:
    explicit Browser(QObject *parent = nullptr);
    void run();

    void createMainWindow();
    QWebEngineProfile *profile();

    static Browser *instance();

private:
    QList<MainWindow *> m_mainWindows;
    QWebEngineProfile *m_profile = nullptr;
};
