#pragma once

#include <QWidget>

class Tab;
class TabBar;
class StackedWidget;
class TabList;
class WebView;

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);
    ~TabWidget();
    void addTab();
    void addTab(const QUrl &url);
    void addTab(WebView *webview);
    int addBackgroundTab(WebView *webview);
    void closeTab(Tab *tab);
    void closeTabsAfter(Tab *tab);
    void closeTabsBefore(Tab *tab);
    void currentTabChanged(Tab *tab);
    void toggleTabList();
private:
    TabBar *m_tabBar = nullptr;
    StackedWidget *m_stakedWidget = nullptr;
    TabList *m_tabList = nullptr;

    void handleTabChanged(Tab *tab);
};
