#include "bookmarks.h"
#include "utils.h"

#include <QComboBox>
#include <QCursor>
#include <QDir>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>

#include <QDebug>

#include <iostream>

/*
 * TODO: currently there are multiple calls to Bookmarks::isBookmarked,
 *       please try to reduce them.
 */

QDomDocument Bookmarks::s_xmlDom = QDomDocument();

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
    readBookmarksFile();
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
    item.address = sql.value(sql.record().indexOf(QStringLiteral("url"))).toString();
    item.description = sql.value(sql.record().indexOf(QStringLiteral("description"))).toString();
    item.folder = sql.value(sql.record().indexOf(QStringLiteral("folder"))).toString();

    return item;
}

void Bookmarks::insertBookmark(const BookmarkItem &item)
{
    QDomElement element = s_xmlDom.createElement(QStringLiteral("item"));
    element.setAttribute(QStringLiteral("title"), item.title);
    element.setAttribute(QStringLiteral("address"), item.address);
    element.setAttribute(QStringLiteral("description"), item.description);

    const QDomNodeList xmlItems = s_xmlDom.elementsByTagName(QStringLiteral("item"));
    for (int i = 0; i < xmlItems.length(); i++) {
        QDomNode xmlItem = xmlItems.at(i);
        QString xmlItemAddress = xmlItem.attributes().namedItem("address").toAttr().value();
        QUrl xmlItemUrl(xmlItemAddress);
        QUrl itemUrl(item.address);
        if (xmlItemUrl.matches(itemUrl, QUrl::StripTrailingSlash)) {
            xmlItem.parentNode().removeChild(xmlItem);
        }
    }

    QString nearestParentFolderName;
    QDomNode nearestParentFolder;

    const QDomNodeList xmlFolders = s_xmlDom.elementsByTagName(QStringLiteral("folder"));
    for (int i = 0; i < xmlFolders.length(); i++) {
        QDomNode xmlFolder = xmlFolders.at(i);
        QStringList xmlFolderName;
        QDomNode parentFolder = xmlFolder;
        while (!parentFolder.isNull()) {
            QString parentFolderName = parentFolder.attributes().namedItem("name").toAttr().value();
            if (parentFolderName.isNull()) {
                break;
            }
            xmlFolderName.prepend(parentFolderName);
            parentFolder = parentFolder.parentNode();
        }

        QString folderName = xmlFolderName.join(QLatin1Char('/'));
        if (folderName == item.folder) {
            xmlFolder.appendChild(element);
            return;
        }

        if (item.folder.startsWith(folderName)) {
            if (nearestParentFolderName.length() < folderName.length()) {
                nearestParentFolderName = folderName;
                nearestParentFolder = xmlFolder;
            }
        }
    }

    // add folder if folder is not already present
    if (nearestParentFolder.isNull()) {
        nearestParentFolder = s_xmlDom.elementsByTagName(QStringLiteral("bookmarks")).at(0);
    }

    QStringList nearestParentFolderPath = nearestParentFolderName.split(QLatin1Char('/'));
    QStringList itemFolderPath = item.folder.split(QLatin1Char('/'));

    nearestParentFolderPath.removeAll(QStringLiteral(""));
    itemFolderPath.removeAll(QStringLiteral(""));

    for (int i = nearestParentFolderPath.length(); i < itemFolderPath.length(); i++) {
        QString currentFolderName = itemFolderPath.at(i);
        QDomElement element = s_xmlDom.createElement(QStringLiteral("folder"));
        element.setAttribute(QStringLiteral("name"), currentFolderName);
        nearestParentFolder = nearestParentFolder.appendChild(element);
        nearestParentFolderName = nearestParentFolderName.append(QStringLiteral("/%1").arg(currentFolderName));
    }

    nearestParentFolder.appendChild(element);
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
    QLineEdit *folder = new QLineEdit;

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

    folder->setText(item.folder);

    save->setObjectName(QStringLiteral("default"));

    connect(remove, &QPushButton::clicked, [item, widget] {
        Bookmarks::removeBookmark(item.address);
        widget->close();
    });

    connect(save, &QPushButton::clicked, [title, description, folder, widget, item] {
        BookmarkItem newItem;
        newItem.title = title->text();
        newItem.address = item.address;
        newItem.description = description->text();
        newItem.folder = folder->text().toLower();
        insertBookmark(newItem);

        widget->close();
    });

    return widget;
}

void Bookmarks::readBookmarksFile()
{
    if (!s_xmlDom.isNull()) {
        return;
    }

    QDir standardLocation(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QFile file(standardLocation.absoluteFilePath(QStringLiteral("bookmarks.xml")));
    if (!file.exists() || !file.open(QFile::ReadOnly)) {
        s_xmlDom.setContent(QStringLiteral("<bookmarks>"
                                           "    <folder name='blogs'></folder>"
                                           "    <folder name='business'></folder>"
                                           "    <folder name='entertainment'></folder>"
                                           "    <folder name='general'></folder>"
                                           "    <folder name='news'></folder>"
                                           "    <folder name='shopping'></folder>"
                                           "    <folder name='social'></folder>"
                                           "    <folder name='sports'></folder>"
                                           "    <folder name='technology'></folder>"
                                           "    <folder name='tools'></folder>"
                                           "    <folder name='travel'></folder>"
                                           "</bookmarks>"));
    } else {
        s_xmlDom.setContent(&file);
        file.close();
    }
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
        bookmarkItem.title = sql.value(sql.record().indexOf(QStringLiteral("title"))).toString();
        bookmarkItem.address = sql.value(sql.record().indexOf(QStringLiteral("url"))).toString();
        bookmarkItem.description = sql.value(sql.record().indexOf(QStringLiteral("description"))).toString();
        bookmarkItem.folder = sql.value(sql.record().indexOf(QStringLiteral("folder"))).toString();

        BookmarkTreeItem *item = new BookmarkTreeItem;
        item->bookmarkItem = bookmarkItem;

        item->setText(0, bookmarkItem.title);
        item->setToolTip(0, bookmarkItem.description);

        m_treeWidget->addTopLevelItem(item);
    }
}
