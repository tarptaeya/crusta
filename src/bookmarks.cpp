#include "bookmarks.h"
#include "utils.h"

#include <QComboBox>
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

    connect(remove, &QPushButton::clicked, [item, widget] {
        Bookmarks::removeBookmark(item.url);
        widget->close();
    });

    connect(save, &QPushButton::clicked, [title, description, folder, widget, item] {
        QSqlQuery sql;
        sql.prepare(QStringLiteral("update bookmarks set title = ?, description = ?, folder = ? where url = ?"));
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
    m_searchBox = new QLineEdit;
    m_folderBox = new QComboBox;
    m_addFolderButton = new QToolButton;
    m_removeFolderButton = new QToolButton;
    m_refreshButton = new QToolButton;

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    m_widget->setLayout(vboxLayout);

    vboxLayout->setContentsMargins(0, 0, 0, 0);

    vboxLayout->addWidget(m_searchBox);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(m_folderBox);
    hboxLayout->addWidget(m_addFolderButton);
    hboxLayout->addWidget(m_removeFolderButton);
    hboxLayout->addWidget(m_refreshButton);
    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(m_treeWidget);

    m_searchBox->setPlaceholderText(QStringLiteral("Search bookmarks..."));
    m_searchBox->setClearButtonEnabled(true);
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

    updateBookmarksWidget();

    connect(m_refreshButton, &QToolButton::clicked, this, &Bookmarks::updateBookmarksWidget);
}

void Bookmarks::updateBookmarksWidget()
{
    s_folderNames.sort();
    for (const QString &folderName : s_folderNames) {
        m_folderBox->addItem(folderName);
    }
}
