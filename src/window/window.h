#pragma once

#include <QMainWindow>

class CentralWidget;

class Window : public QMainWindow
{
    Q_OBJECT
public:
    enum Type {
        Normal,
        Private
    };
    explicit Window(QWidget *parent = nullptr);
    ~Window();

private:
    bool m_isPrivate;
    CentralWidget *m_centralWidget = nullptr;
};
