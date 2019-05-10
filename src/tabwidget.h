#pragma once

#include <QToolButton>
#include <QTabWidget>
#include <QWebEngineHistory>

class Tab;

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);

    int addTab(Tab *tab, bool isBackground = false, const QString &label = QStringLiteral("New Tab"));
    Tab *tabAt(int index);
    void back();
    void forward();
    void navigateToItem(const QWebEngineHistoryItem &item);
    void changeLoadingState();

Q_SIGNALS:
    void urlChanged(const QUrl &url);
    void historyChanged(QWebEngineHistory *history);
    void loadStarted();
    void loadFinished();
    void historyItemInserted();
    void windowCloseRequested();

private:
    QToolButton *m_newTabButton = nullptr;

    void createContextMenu(const QPoint &pos);
};
