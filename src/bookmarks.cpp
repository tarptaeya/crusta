#include "bookmarks.h"
#include "utils.h"

#include <QCompleter>
#include <QCursor>
#include <QDir>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QStandardPaths>

#include <iostream>

QDomDocument Bookmarks::s_xmlDom = QDomDocument();
QMap<QString, BookmarkItem> Bookmarks::s_bookmarks;

Bookmarks::Bookmarks(QObject *parent)
    : QObject (parent)
{
    m_widget = new QWidget;
    m_treeWidget = new QTreeWidget;
    m_treeWidget->setHeaderHidden(true);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    m_widget->setLayout(vboxLayout);

    vboxLayout->setContentsMargins(0, 0, 0, 0);
    vboxLayout->addWidget(m_treeWidget);

    readBookmarksFile();
    refreshBookmarksWidget();

    m_treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_treeWidget, &QTreeWidget::itemDoubleClicked, [this] {
        QTreeWidgetItem *item = m_treeWidget->currentItem();
        QString address = item->data(0, Qt::ToolTipRole).toString();
        if (address.isNull()) {
            return ;
        }

        emit newTabRequested(address);
    });

    connect(m_treeWidget, &QTreeWidget::customContextMenuRequested, [this] {
        QMenu menu;

        QTreeWidgetItem *item = m_treeWidget->currentItem();
        QString address = item->data(0, Qt::ToolTipRole).toString();
        if (!address.isNull()) {
            QAction *edit = menu.addAction(QStringLiteral("Edit"));
            connect(edit, &QAction::triggered, [this, address] {
                QWidget *widget = popupWidget(s_bookmarks.value(address));
                widget->show();
                widget->move(QCursor::pos());

                connect(widget, &QWidget::destroyed, [this] {
                    refreshBookmarks();
                    refreshBookmarksWidget();
                });
            });
        } else {
            QAction *remove = menu.addAction(QStringLiteral("Remove Folder"));
            connect(remove, &QAction::triggered, [this] {
                QStringList folderPath;
                QTreeWidgetItem *item = m_treeWidget->currentItem();
                while (item) {
                    folderPath.prepend(item->text(0));
                    item = item->parent();
                }

                removeBookmarkFolder(folderPath.join(QLatin1Char('/')));
                refreshBookmarks();
                refreshBookmarksWidget();
            });
        }

        menu.exec(QCursor::pos());
    });
}

QWidget *Bookmarks::bookmarksWidget()
{
    return m_widget;
}

BookmarkItem Bookmarks::isBookmarked(const QString &url)
{
    return s_bookmarks[url];
}

void Bookmarks::insertBookmark(const BookmarkItem &item)
{
    s_bookmarks.insert(item.address, item);

    QDomElement element = s_xmlDom.createElement(QStringLiteral("item"));
    element.setAttribute(QStringLiteral("title"), item.title);
    element.setAttribute(QStringLiteral("address"), item.address);
    element.setAttribute(QStringLiteral("description"), item.description);

    removeBookmarkFromXMLDom(item.address);

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
            saveBookmarksFile();
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
    saveBookmarksFile();
}

void Bookmarks::removeBookmark(const QString &url)
{
    removeBookmarkFromXMLDom(url);
    s_bookmarks.remove(url);
    saveBookmarksFile();
}

void Bookmarks::removeBookmarkFolder(const QString &name)
{
    QDomNodeList folders = s_xmlDom.elementsByTagName(QStringLiteral("folder"));
    for (int i = 0; i < folders.length(); i++) {
        QDomNode folder = folders.at(i);
        QStringList folderPath;
        QDomNode parentFolder = folder;
        while (!parentFolder.isNull()) {
            QString parentFolderName = parentFolder.attributes().namedItem("name").toAttr().value();
            if (parentFolderName.isNull()) {
                break;
            }
            folderPath.prepend(parentFolderName);
            parentFolder = parentFolder.parentNode();
        }

        QString folderName = folderPath.join(QLatin1Char('/'));
        if (folderName == name) {
            folder.parentNode().removeChild(folder);
            saveBookmarksFile();
            return;
        }
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
    QStringList folders;
    for (const BookmarkItem &bookmarksItem : s_bookmarks.values()) {
        folders << bookmarksItem.folder;
    }
    QCompleter *completer = new QCompleter(folders);
    completer->setCompletionMode(QCompleter::InlineCompletion);
    completer->setFilterMode(Qt::MatchStartsWith);
    folder->setCompleter(completer);

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

    QString data;

    QDir standardLocation(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QFile file(standardLocation.absoluteFilePath(QStringLiteral("bookmarks.xml")));
    if (file.open(QFile::ReadOnly)) {
        data = file.readAll();
        file.close();
    }

    data = data.trimmed();

    if (data.isEmpty()) {
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
        s_xmlDom.setContent(data);
    }

    refreshBookmarks();
}

void Bookmarks::saveBookmarksFile()
{
    QDir standardLocation(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    QFile file(standardLocation.absoluteFilePath(QStringLiteral("bookmarks.xml")));
    if (file.open(QFile::WriteOnly)) {
        file.write(s_xmlDom.toString().toUtf8());
        file.close();
    }
}

void Bookmarks::removeBookmarkFromXMLDom(const QString &url)
{
    const QDomNodeList xmlItems = s_xmlDom.elementsByTagName(QStringLiteral("item"));
    for (int i = 0; i < xmlItems.length(); i++) {
        QDomNode xmlItem = xmlItems.at(i);
        QString xmlItemAddress = xmlItem.attributes().namedItem("address").toAttr().value();
        QUrl xmlItemUrl(xmlItemAddress);
        QUrl itemUrl(url);
        if (xmlItemUrl.matches(itemUrl, QUrl::StripTrailingSlash)) {
            xmlItem.parentNode().removeChild(xmlItem);
        }
    }
}

void Bookmarks::refreshBookmarks()
{
    s_bookmarks.clear();

    const QDomNodeList xmlItems = s_xmlDom.elementsByTagName(QStringLiteral("item"));
    for (int i = 0; i < xmlItems.length(); i++) {
        QDomNode xmlItem = xmlItems.at(i);
        BookmarkItem item;
        item.title = xmlItem.attributes().namedItem("title").toAttr().value();
        item.address = xmlItem.attributes().namedItem("address").toAttr().value();
        item.description = xmlItem.attributes().namedItem("description").toAttr().value();
        QStringList folderPath;
        QDomNode parentFolder = xmlItem.parentNode();
        while (!parentFolder.isNull()) {
            QString parentFolderName = parentFolder.attributes().namedItem("name").toAttr().value();
            if (parentFolderName.isNull()) {
                break;
            }
            folderPath.prepend(parentFolderName);
            parentFolder = parentFolder.parentNode();
        }

        item.folder = folderPath.join(QLatin1Char('/'));

        s_bookmarks.insert(item.address, item);
    }
}

void Bookmarks::refreshBookmarksWidget()
{
    m_treeWidget->clear();

    QDomNode bookmarksNode = s_xmlDom.firstChild();
    QDomNode folder = bookmarksNode.firstChild();
    while (!folder.isNull()) {
        QTreeWidgetItem *treeFolderItem = xmlDomTraverse(folder);
        m_treeWidget->addTopLevelItem(treeFolderItem);
        folder = folder.nextSibling();
    }
}

QTreeWidgetItem *Bookmarks::xmlDomTraverse(QDomNode node)
{
    if (node.isNull()) {
        return nullptr;
    }

    if (node.toElement().tagName() == QStringLiteral("item")) {
        QTreeWidgetItem *itemItem = new QTreeWidgetItem;
        itemItem->setText(0, node.attributes().namedItem(QStringLiteral("title")).toAttr().value());
        itemItem->setIcon(0, QIcon::fromTheme(QStringLiteral("globe")));
        itemItem->setData(0, Qt::ToolTipRole, node.attributes().namedItem(QStringLiteral("address")).toAttr().value());
        return itemItem;
    }

    QTreeWidgetItem *folderItem = new QTreeWidgetItem;
    folderItem->setText(0, node.attributes().namedItem(QStringLiteral("name")).toAttr().value());
    folderItem->setIcon(0, QIcon::fromTheme(QStringLiteral("folder")));

    QDomNode childFolder = node.firstChild();
    while (!childFolder.isNull()) {
        QTreeWidgetItem *childItem = xmlDomTraverse(childFolder);
        folderItem->addChild(childItem);
        childFolder = childFolder.nextSibling();
    }

    return folderItem;
}
