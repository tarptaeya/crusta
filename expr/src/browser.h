#pragma once

#include <QObject>
#include <QWebEngineProfile>

class Window;

class Browser : public QObject
{
public:
    explicit Browser(QObject *parent = nullptr);
    void run();

    void createWindow();
    QWebEngineProfile *profile();

    static Browser *instance();

private:
    QList<Window *> m_windows;
    QWebEngineProfile *m_profile = nullptr;
};
