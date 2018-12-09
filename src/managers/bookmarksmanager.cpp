#include "bookmarksmanager.h"
#include "appmanager.h"
#include "database.h"
#include "bookmarksitem.h"

void BookmarksManager::showBookmarksManager(QWidget *baseWidget)
{
    while (QWidget *w = baseWidget->findChild<QWidget *>()) {
        delete w;
    }

    QVBoxLayout *vbox = new QVBoxLayout();
    baseWidget->setLayout(vbox);

    QLabel *titleLable = new QLabel("Bookmarks");
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

    treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    treeWidget->setAlternatingRowColors(true);

    QList<QTreeWidgetItem *> categoryItems;

    const QStringList categories = appManager->database()->loadCategories();
    for (const QString &category : categories) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, category);
        categoryItems.append(item);
        treeWidget->addTopLevelItem(item);
    }

    int row = 0;
    const QList<BookmarksItem> bookmarksItems = appManager->database()->loadBookmarks();
    for (const BookmarksItem &bookmarksItem : bookmarksItems) {
        QTreeWidgetItem *item = new QTreeWidgetItem();
        for (int i = 0; i < categoryItems.size(); i++) {
            if (categoryItems.at(i)->text(0) == bookmarksItem.category()) {
                delete item;
                item = new QTreeWidgetItem(categoryItems.at(i));
                break;
            }
        }
        QPixmap pixmap;
        pixmap.loadFromData(bookmarksItem.favicon());
        if (pixmap.isNull()) {
            pixmap.load(":/icons/small_globe.svg");
        }
        item->setIcon(0, QIcon(pixmap));
        item->setText(0, bookmarksItem.title());
        item->setText(1, bookmarksItem.url());
        treeWidget->insertTopLevelItem(0, item);
        row++;
    }

    connect(treeWidget, &QTreeWidget::customContextMenuRequested, treeWidget, [treeWidget](const QPoint &pos) {
        QTreeWidgetItem *item = treeWidget->currentItem();
        if (item->childCount() == 0) {
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
            appManager->database()->removeBookmarksEntry(item->text(1));
            delete item;
        });

        menu->exec(treeWidget->mapToGlobal(pos));
        menu->deleteLater();
    });

}
