#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include <QIcon>
#include <QModelIndex>

struct HistoryEntry
{
    QString title;
    QString address;
    QIcon icon;
    QDateTime last_visited;
};

class HistoryModel : public QAbstractTableModel
{
    QVector<HistoryEntry> m_entries;
public:
    explicit HistoryModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void add_entry(const HistoryEntry &entry);
    void update_entry(const HistoryEntry &entry);
};
