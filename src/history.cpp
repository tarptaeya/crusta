#include "history.h"
#include "utils.h"

#include <QHeaderView>
#include <QMenu>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QUrl>
#include <QVariant>
#include <QVBoxLayout>

#include <iostream>

History::History(QObject *parent)
    : QObject (parent)
{
    m_historyWidget = new QWidget;
    m_treeWidget = new QTreeWidget;
    m_searchBox = new QLineEdit;

    updateTopLevelItems();
    createHistoryWidget();

    connect(this, &History::historyChanged, this, &History::updateHistoryWidget);
    connect(m_searchBox, &QLineEdit::returnPressed, this, [this] {
        const QString &text = m_searchBox->text();

        QList<QTreeWidgetItem *> allItems = m_treeWidget->findItems(QStringLiteral(""), Qt::MatchContains | Qt::MatchRecursive);
        for (QTreeWidgetItem *item : allItems) {
            if (!item->parent()) {
                item->setExpanded(true);
                continue;
            }
            item->setHidden(true);
        }

        QList<QTreeWidgetItem *> items = m_treeWidget->findItems(text, Qt::MatchContains | Qt::MatchRecursive);
        for (QTreeWidgetItem *item : items) {
            item->setHidden(false);
        }
    });
}

void History::insertItem(const HistoryItem &item)
{
    QSqlQuery query;
    query.prepare(QStringLiteral("insert or replace into history (icon, title, url) values (?, ?, ?)"));
    query.addBindValue(Utils::iconToByteArray(item.icon));
    query.addBindValue(item.title);
    query.addBindValue(item.url);
    if (!query.exec()) {
        std::cerr << "unable to insert history item: " << query.lastError().text().toStdString() << std::endl;
    }
}

void History::removeItem(const QString &title)
{
    QSqlQuery query;
    query.prepare(QStringLiteral("delete from history where title = ?"));
    query.addBindValue(title);
    if (!query.exec()) {
        std::cerr << "unable to remove item: " << query.lastError().text().toStdString() << std::endl;
    }
}

QWidget *History::historyWidget()
{
    return m_historyWidget;
}

QList<HistoryItem> History::getAllHistory()
{
    QSqlQuery query(QStringLiteral("select * from history order by timestamp desc"));
    QList<HistoryItem> list;
    int iconField = query.record().indexOf(QStringLiteral("icon"));
    int titleField = query.record().indexOf(QStringLiteral("title"));
    int urlField = query.record().indexOf(QStringLiteral("url"));
    int timestampField = query.record().indexOf(QStringLiteral("timestamp"));

    while (query.next()) {
        HistoryItem item;
        item.icon = Utils::iconFromByteArray(query.value(iconField).toByteArray());
        item.title = query.value(titleField).toString();
        item.url = query.value(urlField).toString();
        item.timestamp = query.value(timestampField).toDateTime();

        list << item;
    }

    return list;
}

void History::createHistoryWidget()
{
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    m_historyWidget->setLayout(vboxLayout);

    m_searchBox->setPlaceholderText(QStringLiteral("Search history..."));
    m_searchBox->setClearButtonEnabled(true);
    vboxLayout->addWidget(m_searchBox);

    vboxLayout->addWidget(m_treeWidget);

    m_treeWidget->header()->hide();

    updateHistoryWidget();

    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_treeWidget, &QTreeWidget::customContextMenuRequested, this, [this] (const QPoint &pos) {
        QTreeWidgetItem *item = m_treeWidget->itemAt(pos);
        if (!item || !item->parent()) {
            return ;
        }

        QMenu menu;
        QAction *openInNewTab = menu.addAction(QStringLiteral("Open in new tab"));
        QAction *remove = menu.addAction(QStringLiteral("Remove"));

        connect(openInNewTab, &QAction::triggered, this, [this, item] {
            QString url = item->data(0, Qt::ToolTipRole).toString();
            emit newTabRequested(QUrl::fromEncoded(url.toUtf8()));
        });

        connect(remove, &QAction::triggered, this, [this, item] {
            removeItem(item->text(0));
            updateHistoryWidget();
        });
        menu.exec(m_treeWidget->mapToGlobal(pos));
    });
}

void History::updateHistoryWidget()
{
    m_treeWidget->clear();

    updateTopLevelItems();

    QTreeWidgetItem *previous = nullptr;
    const QList<HistoryItem> historyItems = getAllHistory();
    for (const HistoryItem &historyItem : historyItems) {
        if (previous && previous->text(0) == historyItem.title) {
            continue;
        }

        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setIcon(0, historyItem.icon);
        item->setText(0, historyItem.title);
        item->setData(0, Qt::ToolTipRole, historyItem.url);
        previous = item;

        qint64 days = historyItem.timestamp.daysTo(QDateTime::currentDateTime());
        if (days == 0) {
            m_todayItem->addChild(item);
        } else if (days <= 7) {
            m_weekItem->addChild(item);
        } else if (days <= 30) {
            m_monthItem->addChild(item);
        } else {
            m_olderItem->addChild(item);
        }
    }

    m_todayItem->setExpanded(true);
}

void History::updateTopLevelItems()
{
    m_todayItem = new QTreeWidgetItem;
    m_weekItem = new QTreeWidgetItem;
    m_monthItem = new QTreeWidgetItem;
    m_olderItem = new QTreeWidgetItem;

    m_todayItem->setText(0, QStringLiteral("Today"));
    m_todayItem->setFont(0, QFont("", -1, QFont::Bold));
    m_treeWidget->addTopLevelItem(m_todayItem);

    m_weekItem->setText(0, QStringLiteral("This week"));
    m_weekItem->setFont(0, QFont("", -1, QFont::Bold));
    m_treeWidget->addTopLevelItem(m_weekItem);

    m_monthItem->setText(0, QStringLiteral("This month"));
    m_monthItem->setFont(0, QFont("", -1, QFont::Bold));
    m_treeWidget->addTopLevelItem(m_monthItem);

    m_olderItem->setText(0, QStringLiteral("Older"));
    m_olderItem->setFont(0, QFont("", -1, QFont::Bold));
    m_treeWidget->addTopLevelItem(m_olderItem);
}
