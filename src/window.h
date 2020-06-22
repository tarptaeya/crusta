#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class TabWidget;

class Window : public QMainWindow
{
    TabWidget *m_tabWidget = nullptr;
public:
    Window(QWidget *parent = nullptr);
};

#endif // WINDOW_H
