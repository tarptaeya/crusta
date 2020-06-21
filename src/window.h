#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class ToolBar;
class TabWidget;

class Window : public QMainWindow
{
    ToolBar *m_toolBar = nullptr;
    TabWidget *m_tabWidget = nullptr;
public:
    Window(QWidget *parent = nullptr);
};

#endif // WINDOW_H
