#pragma once

#include <QWidget>

class WebView;
class TabListItem;
class TabWidget;

class Tab : public QWidget
{
    Q_OBJECT
public:
    enum DisplayMode {
        Desktop,
        IPhone,
        Android,
        Custom
    };
    explicit Tab(QWidget *parent = nullptr, WebView *webview = nullptr);
    WebView *webview() const;
    TabListItem *tabListItem() const;
    void setTabListItem(TabListItem *tabListItem);
    void closeTab();
    void setVirtualTabWidget(TabWidget *tabWidget);
private:
    WebView *m_webView = nullptr;
    TabListItem *m_tabListItem = nullptr;
    TabWidget *m_tabWidget = nullptr;
};
