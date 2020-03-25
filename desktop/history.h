#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include <QIcon>
#include <QModelIndex>
#include <QTreeView>
#include <QWidget>

struct HistoryEntry
{
    QString title;
    QString address;
    QIcon icon;
    QDateTime last_visited;

    inline bool operator==(const HistoryEntry &entry) {
        return address == entry.address;
    }
};

class HistoryModel : public QAbstractTableModel
{
    QVector<HistoryEntry> m_entries;
public:
    enum Role {
        AddressRole = Qt::UserRole + 1,
    };
    explicit HistoryModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void add_entry(const HistoryEntry &entry);
    void remove_entry(int offset);
};

class HistoryWidget : public QWidget
{
    QTreeView *m_tree_view = nullptr;

    void show_context_menu(const QPoint &pos);
public:
    explicit HistoryWidget(QWidget *parent = nullptr);
};
