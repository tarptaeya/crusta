#pragma once

#include <QMainWindow>
#include <QSplitter>

class SideBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void createWindow();
    void show();
    void closeEvent(QCloseEvent *event) override;
    bool event(QEvent *event) override;

Q_SIGNALS:
    void newMainWindowRequested();
    void mainWindowWillClose();

private:
    QSplitter *m_centralWidget = nullptr;
    SideBar *m_sideBar = nullptr;
};
