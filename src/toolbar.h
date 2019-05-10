#pragma once

#include <QAction>
#include <QLineEdit>
#include <QToolBar>
#include <QToolButton>
#include <QWebEngineHistory>

class TabWidget;

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);

    void setUrl(const QUrl &url);
    void loadStarted();
    void loadFinished();

    void setHistory(QWebEngineHistory *history);
    void setMenu(QMenu *menu);
    void setTabWidget(TabWidget *tabWidget);

Q_SIGNALS:
    void backRequested();
    void forwardRequested();
    void navigationToItemRequest(const QWebEngineHistoryItem &item);
    void loadingStateChangeRequest();

private:
    TabWidget *m_tabWidget = nullptr;

    QToolButton *m_backButton = nullptr;
    QToolButton *m_forwardButton = nullptr;
    QToolButton *m_refreshButton = nullptr;
    QToolButton *m_homeButton = nullptr;
    QToolButton *m_downloadButton = nullptr;
    QToolButton *m_menuButton = nullptr;

    QLineEdit *m_addressBar = nullptr;

    QMenu *m_backMenu = nullptr;
    QMenu *m_forwardMenu = nullptr;

    QAction *m_bookmarksAction = nullptr;

    void setupAddressBar();
};
