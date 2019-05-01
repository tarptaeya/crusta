#pragma once

#include <QHBoxLayout>
#include <QMainWindow>

class SideBar;
class TabWidget;
class ToolBar;

class Window : public QMainWindow
{
public:
    explicit Window(QWidget *parent = nullptr);

    void createTabWidget();

private:
    SideBar *m_sideBar = nullptr;
    ToolBar *m_toolBar = nullptr;

    QWidget *m_centralWidget = nullptr;
    QHBoxLayout *m_centralWidgetLayout = nullptr;

    QList<TabWidget *> m_tabWidgets;
};
