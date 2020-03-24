#include "history.h"

#include <QBuffer>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

HistoryModel::HistoryModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    QSqlQuery query;
    query.prepare(QStringLiteral("SELECT * FROM history ORDER BY last_visited DESC"));
    if (!query.exec()) {
        qDebug() << query.lastError();
        return;
    }

    while (query.next()) {
        HistoryEntry entry;
        entry.title = query.value(QStringLiteral("title")).toString();
        entry.address = query.value(QStringLiteral("address")).toString();
        entry.last_visited = query.value(QStringLiteral("last_visited")).toDateTime();
        QByteArray out_byte_array = query.value(QStringLiteral("icon")).toByteArray();
        QPixmap out_pixmap = QPixmap();
        out_pixmap.loadFromData(out_byte_array);
        entry.icon = QIcon(out_pixmap);

        m_entries.append(entry);
    }
}

int HistoryModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_entries.count();
}

int HistoryModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 2;
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 && index.row() >= m_entries.count())
        return QVariant();

    HistoryEntry entry = m_entries.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return entry.title;
        case 1: return entry.address;
        default: break;
        }
    case Qt::DecorationRole:
        if (index.column() == 0)
            return entry.icon;
        break;
    }

    return QVariant();
}

QVariant HistoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return QStringLiteral("Title");
        case 1: return QStringLiteral("Address");
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

void HistoryModel::add_entry(const HistoryEntry &entry)
{
    if (m_entries.count() > 0 && m_entries.at(0).address == entry.address) {
        return;
    }

    QByteArray in_byte_array;
    QBuffer in_buffer( &in_byte_array );
    in_buffer.open( QIODevice::WriteOnly );
    entry.icon.pixmap(16, 16).save( &in_buffer, "PNG" );

    QSqlQuery query;
    query.prepare(QStringLiteral("INSERT INTO history (title, address, icon, last_visited) VALUES (?, ?, ?, ?)"));
    query.addBindValue(entry.title);
    query.addBindValue(entry.address);
    query.addBindValue(in_byte_array);
    query.addBindValue(entry.last_visited);

    if (!query.exec()) {
        qDebug() << query.lastError();
        return;
    }

    beginInsertRows(QModelIndex(), 0, 0);
    m_entries.prepend(entry);
    endInsertRows();
}

void HistoryModel::update_entry(const HistoryEntry &entry)
{
    QByteArray in_byte_array;
    QBuffer in_buffer( &in_byte_array );
    in_buffer.open( QIODevice::WriteOnly );
    entry.icon.pixmap(16, 16).save( &in_buffer, "PNG" );

    QSqlQuery query;
    query.prepare(QStringLiteral("UPDATE history SET title=?, icon=? WHERE address=?"));
    query.addBindValue(entry.title);
    query.addBindValue(in_byte_array);
    query.addBindValue(entry.address);

    if (!query.exec()) {
        qDebug() << query.lastError();
        return;
    }

    for (int i = 0; i < m_entries.count(); i++) {
        if (m_entries[i].address == entry.address) {
            m_entries[i].title = entry.title;
            m_entries[i].icon = entry.icon;
            QModelIndex idx = index(i, 0);
            emit dataChanged(idx, idx);
        }
    }
}
