#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QTabBar>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>

class CentralWidget;

class BrowserWindow : public QMainWindow
{
    CentralWidget *m_central_widget = nullptr;
    void setup_menubar();
public:
    explicit BrowserWindow(QWidget *parent = nullptr);
};
