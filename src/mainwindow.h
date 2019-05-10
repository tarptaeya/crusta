#pragma once

#include "history.h"

#include <QMainWindow>
#include <QSplitter>

class SideBar;
class Tab;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    Tab *createWindow();
    void show();
    void closeEvent(QCloseEvent *event) override;
    bool event(QEvent *event) override;

Q_SIGNALS:
    void newMainWindowRequested();
    void mainWindowWillClose();

private:
    QSplitter *m_centralWidget = nullptr;
    SideBar *m_sideBar = nullptr;
    History *m_history = nullptr;
};
