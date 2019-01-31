#ifndef CR_BROWSER_WINDOW_H
#define CR_BROWSER_WINDOW_H

#include <QMainWindow>

class StatusBar;
class Tab;
class TabWidget;

class BrowserWindow : public QMainWindow
{
public:
    explicit BrowserWindow(Tab *tab, QWidget *parent = nullptr);

    StatusBar *statusBar();
    TabWidget *tabWidget();

    void loadSettings();

    void closeEvent(QCloseEvent *event);

    void openFile();

private:
    StatusBar *m_statusBar = nullptr;
    TabWidget *m_tabWidget = nullptr;
};

#endif
