#pragma once

#include <QWidget>

class TabBar;
class StackedWidget;
class TabList;

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);
    ~TabWidget();
    void addTab();
    void addTab(const QUrl &url);
    void toggleTabList();
private:
    TabBar *m_tabBar = nullptr;
    StackedWidget *m_stakedWidget = nullptr;
    TabList *m_tabList = nullptr;
};
