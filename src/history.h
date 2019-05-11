#pragma once

#include <QDateTime>
#include <QIcon>
#include <QLineEdit>
#include <QObject>
#include <QTreeWidget>

struct HistoryItem
{
    QIcon icon;
    QString title;
    QString url;
    QDateTime timestamp;
};

class History : public QObject
{
    Q_OBJECT

public:
    explicit History(QObject *parent = nullptr);

    static void insertItem(const HistoryItem &item);
    static void removeItem(const QString &title);
    QWidget *historyWidget();

    QList<HistoryItem> getAllHistory() const;
    void updateHistoryWidget();
    void updateTopLevelItems();

Q_SIGNALS:
    void historyChanged();
    void newTabRequested(const QUrl &url);

private:
    QWidget *m_historyWidget = nullptr;
    QTreeWidget *m_treeWidget = nullptr;
    QTreeWidgetItem *m_todayItem = nullptr;
    QTreeWidgetItem *m_weekItem = nullptr;
    QTreeWidgetItem *m_monthItem = nullptr;
    QTreeWidgetItem *m_olderItem = nullptr;
    QLineEdit *m_searchBox = nullptr;

    void createHistoryWidget();
};
