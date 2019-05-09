#include "history.h"

#include <QLineEdit>
#include <QSqlError>
#include <QSqlQuery>
#include <QTreeWidget>
#include <QVariant>
#include <QVBoxLayout>
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

QWidget *History::historyWidget()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(vboxLayout);

    QLineEdit *searchBox = new QLineEdit;
    searchBox->setPlaceholderText(QStringLiteral("Search history..."));
    vboxLayout->addWidget(searchBox);

    QTreeWidget *tree = new QTreeWidget;
    vboxLayout->addWidget(tree);

    tree->setHeaderLabel(QStringLiteral("Title"));

    return widget;
}
