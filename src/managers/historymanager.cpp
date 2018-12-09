#include "historymanager.h"
#include "historyitem.h"
#include "appmanager.h"
#include "database.h"

void HistoryManager::showHistoryManager(QWidget *baseWidget)
{
    while (QWidget *w = baseWidget->findChild<QWidget *>()) {
        delete w;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    baseWidget->setLayout(vbox);

    QLabel *titleLable = new QLabel("History");
    QFont font;
    font.setBold(true);
    font.setPointSize(25);
    titleLable->setFont(font);
    vbox->addWidget(titleLable);

    QTreeWidget *treeWidget = new QTreeWidget();
    QStringList headers;
    headers << "Title" << "URL";
    treeWidget->setHeaderLabels(headers);
    treeWidget->setColumnWidth(0, 200);
    vbox->addWidget(treeWidget);

    QTreeWidgetItem *todayItem = new QTreeWidgetItem();
    QTreeWidgetItem *weekItem = new QTreeWidgetItem();
    QTreeWidgetItem *olderItem = new QTreeWidgetItem();
    todayItem->setText(0, "Today");
    weekItem->setText(0, "Earlier this week");
    olderItem->setText(0, "Older");
    treeWidget->addTopLevelItem(todayItem);
    treeWidget->addTopLevelItem(weekItem);
    treeWidget->addTopLevelItem(olderItem);

    todayItem->setExpanded(true);
    treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    treeWidget->setAlternatingRowColors(true);

    int row = 0;
    const QList<HistoryItem> historyItems = appManager->database()->loadHistory();
    for (const HistoryItem &historyItem : historyItems) {
        QTreeWidgetItem *item;
        QDate date = QDateTime::fromTime_t(historyItem.timestamp()).date();
        if (date == QDate::currentDate()) {
            item = new QTreeWidgetItem(todayItem);
        } else if (date > QDate::currentDate().addDays(-7)) {
            item = new QTreeWidgetItem(weekItem);
        } else  {
            item = new QTreeWidgetItem(olderItem);
        }
        QPixmap pixmap;
        pixmap.loadFromData(historyItem.favicon());
        if (pixmap.isNull()) {
            pixmap.load(":/icons/small_globe.svg");
        }
        item->setIcon(0, QIcon(pixmap));
        item->setText(0, historyItem.title());
        item->setText(1, historyItem.url());
        treeWidget->insertTopLevelItem(0, item);
        row++;
    }

    connect(treeWidget, &QTreeWidget::customContextMenuRequested, treeWidget, [treeWidget, todayItem, weekItem, olderItem](const QPoint &pos) {
        QTreeWidgetItem *item = treeWidget->currentItem();
        if (item == todayItem || item == weekItem || item == olderItem) {
            return;
        }

        QMenu *menu = new QMenu();
        QAction *copyAction = new QAction("Copy address");
        QAction *removeAction = new QAction("Delete");
        menu->addAction(copyAction);
        menu->addAction(removeAction);

        connect(copyAction, &QAction::triggered, treeWidget, [item] {
            qApp->clipboard()->setText(item->text(1));
        });

        connect(removeAction, &QAction::triggered, treeWidget, [item] {
            appManager->database()->removeHistoryEntry(item->text(1));
            delete item;
        });

        menu->exec(treeWidget->mapToGlobal(pos));
        menu->deleteLater();
    });
}
