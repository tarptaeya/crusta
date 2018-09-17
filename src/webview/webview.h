#pragma once

#include <QWebEngineView>

class WebPage;
class TabWidget;

class WebView : public QWebEngineView
{
public:
    explicit WebView(QWidget *parent = nullptr);
    ~WebView();
    void loadStartupUrl();
    void setVirtualTabWidget(TabWidget *tabWidget);
    void search(const QString &text);
    bool isLoading() const;
    QString hoveredLink() const;
    int loadingTime() const;
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
private:
    WebPage *m_webPage = nullptr;
    TabWidget *m_tabWidget = nullptr;
    bool m_isLoading = false;
    QString m_hoveredLink;
    int m_loadingTime;

    void handleLoadStarted();
    void handleLoadFinished();
    void handleLinkHovered(const QString &url);
    void showContextMenu(const QPoint &pos);
    QByteArray grabAsByteArray();
    bool matchesCurrentUrl(const QUrl &otherUrl);
};
