#include "browser.h"
#include "history.h"

#include <QBuffer>
#include <QDebug>
#include <QMenu>
#include <QSqlError>
#include <QSqlQuery>
#include <QStyleFactory>
#include <QVBoxLayout>

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
    for (int i = 0; i < m_entries.count(); i++) {
        if (m_entries[i].address == entry.address) {
            remove_entry(i);
            add_entry(entry);
            return;
        }
    }

    if (m_entries.count() > 0 && m_entries.at(0).address == entry.address) {
        return;
    }

    QByteArray in_byte_array;
    QBuffer in_buffer( &in_byte_array );
    in_buffer.open( QIODevice::WriteOnly );
    entry.icon.pixmap(16, 16).save( &in_buffer, "PNG" );

    QSqlQuery query;
    query.prepare(QStringLiteral("REPLACE INTO history (title, address, icon, last_visited) VALUES (?, ?, ?, ?)"));
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

void HistoryModel::remove_entry(int offset)
{
    const QString address = m_entries.at(offset).address;

    QSqlQuery query;
    query.prepare(QStringLiteral("DELETE FROM history WHERE address = ?"));
    query.addBindValue(address);

    if (!query.exec()) {
        qDebug() << query.lastError();
        return;
    }

    QMutableVectorIterator<HistoryEntry> it(m_entries);
    while (it.hasNext()) {
        HistoryEntry entry = it.next();
        if (entry.address == address) {
            int index = m_entries.indexOf(entry);
            beginRemoveRows(QModelIndex(), index, index);
            it.remove();
            endRemoveRows();
        }
    }
}

void HistoryWidget::show_context_menu(const QPoint &pos)
{
    QModelIndex index = m_tree_view->indexAt(pos);
    if (!index.isValid()) {
        return;
    }

    QMenu *menu = new QMenu;
    QAction *remove_entry = menu->addAction(QStringLiteral("Remove"));

    connect(remove_entry, &QAction::triggered, [index] {
        browser->history_model()->remove_entry(index.row());
    });

#ifdef Q_OS_MACOS
    menu->setStyle(QStyleFactory::create(QStringLiteral("macintosh")));
#endif
    menu->exec(m_tree_view->mapToGlobal(pos));
}

HistoryWidget::HistoryWidget(QWidget *parent)
    : QWidget(parent)
{
    m_tree_view = new QTreeView;
    m_tree_view->setModel(browser->history_model());

    QVBoxLayout *vbox = new QVBoxLayout;
    setLayout(vbox);
    vbox->addWidget(m_tree_view);

    m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tree_view, &QTreeView::customContextMenuRequested, this, &HistoryWidget::show_context_menu);
}
