#pragma once

#include <QWebEngineHistory>
#include <QWebEnginePage>
#include <QWebEngineView>

class Tab;
class WebPage;

class WebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = nullptr);

    bool isLoading() const;
    void loadHome();
    void loadNewTabPage();
    WebView *createWindow(QWebEnginePage::WebWindowType type);

Q_SIGNALS:
    void historyChanged(QWebEngineHistory *history);
    void historyItemInserted();
    void addTabRequested(Tab *tab, bool isBackground = false);

private:
    WebPage *m_webPage = nullptr;
    bool m_isLoading = false;

    void insertHistoryItem();
};
