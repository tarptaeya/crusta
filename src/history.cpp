#include "history.h"
#include "utils.h"

#include <QLineEdit>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QVBoxLayout>
#include <iostream>

History::History()
{
    m_historyWidget = new QWidget;
    m_treeWidget = new QTreeWidget;
    m_todayItem = new QTreeWidgetItem;
    m_weekItem = new QTreeWidgetItem;
    m_monthItem = new QTreeWidgetItem;
    m_olderItem = new QTreeWidgetItem;

    createHistoryWidget();

    connect(this, &History::historyChanged, this, &History::updateHistoryWidget);
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

    emit historyChanged();
}

QWidget *History::historyWidget()
{
    return m_historyWidget;
}

QList<HistoryItem> History::getAllHistory() const
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

    QLineEdit *searchBox = new QLineEdit;
    searchBox->setPlaceholderText(QStringLiteral("Search history..."));
    searchBox->setClearButtonEnabled(true);
    vboxLayout->addWidget(searchBox);

    vboxLayout->addWidget(m_treeWidget);

    m_treeWidget->setHeaderLabel(QStringLiteral("Title"));

    updateHistoryWidget();
}

void History::updateHistoryWidget()
{
    m_treeWidget->clear();

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

    QTreeWidgetItem *previous = nullptr;
    const QList<HistoryItem> historyItems = getAllHistory();
    for (const HistoryItem &historyItem : historyItems) {
        if (previous && previous->text(0) == historyItem.title) {
            continue;
        }

        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setIcon(0, historyItem.icon);
        item->setText(0, historyItem.title);
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
