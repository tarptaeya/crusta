#ifndef CR_BROWSER_WINDOW_H
#define CR_BROWSER_WINDOW_H

#include <QMainWindow>

class StatusBar;
class TabWidget;

class BrowserWindow : public QMainWindow
{
public:
    explicit BrowserWindow(const QString &startPageUrl, QWidget *parent = nullptr);

    StatusBar *statusBar();
    TabWidget *tabWidget();

    void loadSettings();

    void closeEvent(QCloseEvent *event);

private:
    StatusBar *m_statusBar = nullptr;
    TabWidget *m_tabWidget = nullptr;
};

#endif
