#include "common-defs.h"
#include "browserwindow.h"
#include "database.h"
#include "historyitem.h"
#include "historymanager.h"
#include "mainapplication.h"
#include "tabwidget.h"

#include <QAction>
#include <QHBoxLayout>
#include <QMenu>
#include <QVBoxLayout>

#include <QPointer>

HistoryManager::HistoryManager(QWidget *parent)
    : QWidget (parent)
{
    m_treeWidget = new QTreeWidget(this);
    QStringList headerLabels;
    headerLabels << QSL("Title") << QSL("Address");
    m_treeWidget->setHeaderLabels(headerLabels);
    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    vboxLayout->addWidget(m_treeWidget);

    updateEntries();

    setLayout(vboxLayout);

    connect(m_treeWidget, &QTreeWidget::customContextMenuRequested, this, &HistoryManager::showContextMenu);
}

void HistoryManager::updateEntries()
{
    m_treeWidget->clear();

    QMap<QDate, QTreeWidgetItem *> map;

    QList<HistoryItem> entries = appManager->dataBase()->history();
    for (const HistoryItem &entry : qAsConst(entries)) {
        QDateTime dateTime(entry.dateTime);

        QTreeWidgetItem *topItem = nullptr;
        if (map.contains(dateTime.date())) {
            topItem = map.value(dateTime.date());
        } else {
            topItem = new QTreeWidgetItem;
            map.insert(dateTime.date(), topItem);
            topItem->setText(0, dateTime.date().toString());
            m_treeWidget->addTopLevelItem(topItem);
        }

        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, entry.title);
        item->setText(1, entry.url);
        topItem->addChild(item);
    }
}

void HistoryManager::showContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = m_treeWidget->itemAt(pos);
    if (!item) {
        return;
    }

    const QString address = item->text(2);

    QMenu menu(this);
    QAction *open = new QAction(QSL("Open in New Tab"));
    QAction *remove = new QAction(QSL("Remove Entry"));
    menu.addAction(open);
    menu.addAction(remove);

    connect(open, &QAction::triggered, this, [address] { appManager->currentWindow()->tabWidget()->addTab(address); });
    connect(remove, &QAction::triggered, this, [address, item] {
        appManager->dataBase()->removeHistory(address);
        delete item;
    });

    menu.exec(m_treeWidget->mapToGlobal(pos));
}
