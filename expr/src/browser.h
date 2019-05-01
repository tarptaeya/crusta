#pragma once

#include <QObject>

class Window;

class Browser : QObject
{
public:
    explicit Browser(QObject *parent = nullptr);
    void run();

    void createWindow();

private:
    QList<Window *> m_windows;
};
