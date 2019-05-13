#include "bookmarks.h"
#include "utils.h"

#include <QComboBox>
#include <QCursor>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <iostream>

/*
 * TODO: currently there are multiple calls to Bookmarks::isBookmarked,
 *       please try to reduce them.
 */

QStringList Bookmarks::s_folderNames = QStringList() << QStringLiteral("Blogs")
                                                     << QStringLiteral("Business")
                                                     << QStringLiteral("Entertainment")
                                                     << QStringLiteral("General")
                                                     << QStringLiteral("News")
                                                     << QStringLiteral("Shopping")
                                                     << QStringLiteral("Social")
                                                     << QStringLiteral("Sports")
                                                     << QStringLiteral("Technology")
                                                     << QStringLiteral("Tools")
                                                     << QStringLiteral("Travel");

Bookmarks::Bookmarks(QObject *parent)
    : QObject (parent)
{
    setupBookmarksWidget();
}

QWidget *Bookmarks::bookmarksWidget()
{
    return m_widget;
}

BookmarkItem Bookmarks::isBookmarked(const QString &url)
{
    QSqlQuery sql;
    sql.prepare(QStringLiteral("select * from bookmarks where url = ?"));
    sql.addBindValue(url);
    if (!sql.exec()) {
        std::cerr << "unable to check if " << url.toStdString() << " is bookmarked: " << sql.lastError().text().toStdString() << std::endl;
        return BookmarkItem();
    }
    if (!sql.next()) {
        return BookmarkItem();
    }

    BookmarkItem item;
    item.icon = Utils::iconFromByteArray(sql.value(sql.record().indexOf(QStringLiteral("icon"))).toByteArray());
    item.title = sql.value(sql.record().indexOf(QStringLiteral("title"))).toString();
    item.url = sql.value(sql.record().indexOf(QStringLiteral("url"))).toString();
    item.description = sql.value(sql.record().indexOf(QStringLiteral("description"))).toString();
    item.folder = sql.value(sql.record().indexOf(QStringLiteral("folder"))).toString();

    return item;
}

void Bookmarks::insertBookmark(const BookmarkItem &item)
{
    QSqlQuery sql;
    sql.prepare(QStringLiteral("insert or replace into bookmarks (icon, title, url, description, folder) values (?, ?, ?, ?, ?)"));
    sql.addBindValue(Utils::iconToByteArray(item.icon));
    sql.addBindValue(item.title);
    sql.addBindValue(item.url);
    sql.addBindValue(item.description);
    sql.addBindValue(item.folder);

    if (!sql.exec()) {
        std::cerr << "unable to insert bookmark: " << sql.lastError().text().toStdString() << std::endl;
        return;
    }
}

void Bookmarks::removeBookmark(const QString &url)
{
    QSqlQuery sql;
    sql.prepare(QStringLiteral("delete from bookmarks where url = ?"));
    sql.addBindValue(url);
    if (!sql.exec()) {
        std::cerr << "unable to delete history: " << sql.lastError().text().toStdString() << std::endl;
    }
}

QWidget *Bookmarks::popupWidget(const BookmarkItem &item)
{
    QWidget *widget = new QWidget;
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setWindowFlag(Qt::Popup);

    QGridLayout *layout = new QGridLayout;
    widget->setLayout(layout);

    QLineEdit *title = new QLineEdit;
    QLineEdit *description = new QLineEdit;
    QComboBox *folder = new QComboBox;

    QPushButton *save = new QPushButton(QStringLiteral("Save"));
    QPushButton *remove = new QPushButton(QStringLiteral("Remove"));

    layout->addWidget(new QLabel(QStringLiteral("Title")), 1, 1);
    layout->addWidget(title, 1, 2);
    layout->addWidget(new QLabel(QStringLiteral("Description")), 2, 1);
    layout->addWidget(description, 2, 2);
    layout->addWidget(new QLabel(QStringLiteral("Folder")), 3, 1);
    layout->addWidget(folder, 3, 2);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    hboxLayout->addWidget(remove);
    hboxLayout->addWidget(save);
    layout->addLayout(hboxLayout, 4, 2);

    title->setText(item.title);
    title->setCursorPosition(0);
    description->setText(item.description);
    description->setCursorPosition(0);

    for (const QString &folderName : s_folderNames) {
        folder->addItem(folderName);
    }

    folder->setCurrentText(item.folder);

    save->setObjectName(QStringLiteral("default"));

    connect(remove, &QPushButton::clicked, [item, widget] {
        Bookmarks::removeBookmark(item.url);
        widget->close();
    });

    connect(save, &QPushButton::clicked, [title, description, folder, widget, item] {
        QSqlQuery sql;
        sql.prepare(QStringLiteral("update bookmarks set icon = ?, title = ?, description = ?, folder = ? where url = ?"));
        sql.addBindValue(Utils::iconToByteArray(item.icon));
        sql.addBindValue(title->text());
        sql.addBindValue(description->text());
        sql.addBindValue(folder->currentText());
        sql.addBindValue(item.url);
        if (!sql.exec()) {
            std::cerr << "unable to update bookmark: " << sql.lastError().text().toStdString() << std::endl;
        }

        widget->close();
    });

    return widget;
}

void Bookmarks::setupBookmarksWidget()
{
    m_widget = new QWidget;
    m_treeWidget = new QTreeWidget;
    m_folderBox = new QComboBox;
    m_addFolderButton = new QToolButton;
    m_removeFolderButton = new QToolButton;
    m_refreshButton = new QToolButton;

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    m_widget->setLayout(vboxLayout);

    vboxLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(m_folderBox);
    hboxLayout->addWidget(m_addFolderButton);
    hboxLayout->addWidget(m_removeFolderButton);
    hboxLayout->addWidget(m_refreshButton);
    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(m_treeWidget);

    m_addFolderButton->setAutoRaise(true);
    m_removeFolderButton->setAutoRaise(true);
    m_refreshButton->setAutoRaise(true);
    m_addFolderButton->setToolTip(QStringLiteral("Add folder"));
    m_removeFolderButton->setToolTip(QStringLiteral("Remove current folder"));
    m_refreshButton->setToolTip(QStringLiteral("Refresh list"));
    m_addFolderButton->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));
    m_removeFolderButton->setIcon(QIcon::fromTheme(QStringLiteral("list-remove")));
    m_refreshButton->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    m_treeWidget->header()->hide();

    for (const QString &folderName : s_folderNames) {
        m_folderBox->addItem(folderName);
    }

    connect(m_refreshButton, &QToolButton::clicked, this, &Bookmarks::showFolderItems);
    connect(m_folderBox, &QComboBox::currentTextChanged, this, [this] { showFolderItems(); });
    connect(m_treeWidget, &QTreeWidget::itemDoubleClicked, this, [this] {
        BookmarkTreeItem *item = dynamic_cast<BookmarkTreeItem *>(m_treeWidget->currentItem());
        if (!item) {
            return ;
        }

        QWidget *widget = Bookmarks::popupWidget(item->bookmarkItem);
        connect(widget, &QWidget::destroyed, this, &Bookmarks::showFolderItems);
        widget->show();

        widget->move(QCursor::pos());
    });
}

void Bookmarks::showFolderItems()
{
    const QString folder = m_folderBox->currentText();
    QSqlQuery sql;
    sql.prepare(QStringLiteral("select * from bookmarks where folder = ? order by title"));
    sql.addBindValue(folder);

    m_treeWidget->clear();

    if (!sql.exec()) {
        std::cerr << "unable to select bookmarks based on folder: " << sql.lastError().text().toStdString() << std::endl;
    }

    while (sql.next()) {
        BookmarkItem bookmarkItem;
        bookmarkItem.icon = Utils::iconFromByteArray(sql.value(sql.record().indexOf(QStringLiteral("icon"))).toByteArray());
        bookmarkItem.title = sql.value(sql.record().indexOf(QStringLiteral("title"))).toString();
        bookmarkItem.url = sql.value(sql.record().indexOf(QStringLiteral("url"))).toString();
        bookmarkItem.description = sql.value(sql.record().indexOf(QStringLiteral("description"))).toString();
        bookmarkItem.folder = sql.value(sql.record().indexOf(QStringLiteral("folder"))).toString();

        BookmarkTreeItem *item = new BookmarkTreeItem;
        item->bookmarkItem = bookmarkItem;

        item->setIcon(0, bookmarkItem.icon);
        item->setText(0, bookmarkItem.title);
        item->setToolTip(0, bookmarkItem.description);

        m_treeWidget->addTopLevelItem(item);
    }
}
