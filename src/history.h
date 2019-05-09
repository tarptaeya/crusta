#pragma once

#include <QDateTime>
#include <QIcon>
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
    explicit History();

    static void insertItem(const HistoryItem &item);
    QWidget *historyWidget();

    QList<HistoryItem> getAllHistory() const;
    void updateHistoryWidget();

Q_SIGNALS:
    void historyChanged();

private:
    QWidget *m_historyWidget = nullptr;
    QTreeWidget *m_treeWidget = nullptr;
    QTreeWidgetItem *m_todayItem = nullptr;
    QTreeWidgetItem *m_weekItem = nullptr;
    QTreeWidgetItem *m_monthItem = nullptr;
    QTreeWidgetItem *m_olderItem = nullptr;

    void createHistoryWidget();
};
