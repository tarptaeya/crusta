#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QMenu>

class TabWidget;
class ToolBar;

class Window : public QMainWindow
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

    QMenu *menu();
    TabWidget *tabWidget();

    void closeEvent(QCloseEvent *event) override;

Q_SIGNALS:
    void newWindowRequested();
    void newMainWindowRequested();
    void historyItemInserted();
    void toggleSideBarRequested();
    void windowWillClose();

private:
    ToolBar *m_toolBar = nullptr;
    TabWidget *m_tabWidget = nullptr;
    QMenu *m_menu = nullptr;

    void setupMenu();
};
