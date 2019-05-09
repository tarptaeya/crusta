#include "history.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <iostream>

void History::insertItem(const HistoryItem &item)
{
    QSqlQuery query;
    query.prepare(QStringLiteral("insert into history (title, url) values (?, ?)"));
    query.addBindValue(item.title);
    query.addBindValue(item.url);
    if (!query.exec()) {
        std::cerr << "unable to insert history item: " << query.lastError().text().toStdString() << std::endl;
    }
}

QTreeWidget *History::historyWidget()
{
    QTreeWidget *widget = new QTreeWidget;
    return widget;
}
