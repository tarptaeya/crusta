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

HistoryManager::HistoryManager(QWidget *parent)
    : QWidget (parent)
{
    m_treeWidget = new QTreeWidget(this);
    QStringList headerLabels;
    headerLabels << QSL("Time") << QSL("Title") << QSL("Address");
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

    QList<HistoryItem> entries = appManager->dataBase()->history();
    for (const HistoryItem &entry : qAsConst(entries)) {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeWidget);
        item->setText(0, entry.dateTime.toString());
        item->setText(1, entry.title);
        item->setText(2, entry.url);
        m_treeWidget->addTopLevelItem(item);
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
