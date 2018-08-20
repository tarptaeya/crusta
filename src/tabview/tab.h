#pragma once

#include <QWidget>

class WebView;
class TabListItem;

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
    explicit Tab(QWidget *parent = nullptr);
    QString title() const;
    WebView *webview() const;
    TabListItem *tabListItem() const;
    void setTabListItem(TabListItem *tabListItem);
    void closeTab();
private:
    QString m_title;
    WebView *m_webView = nullptr;
    TabListItem *m_tabListItem = nullptr;
};
