#pragma once

#include <QComboBox>
#include <QDomDocument>
#include <QIcon>
#include <QLineEdit>
#include <QObject>
#include <QToolButton>
#include <QTreeWidget>
#include <QWidget>

struct BookmarkItem
{
    QString title;
    QString address;
    QString description;
    QString folder = QStringLiteral("general");
};

class BookmarkTreeItem : public QTreeWidgetItem
{
public:
    BookmarkItem bookmarkItem;
};

class Bookmarks : public QObject
{
public:
    explicit Bookmarks(QObject *parent = nullptr);

    QWidget *bookmarksWidget();

    static BookmarkItem isBookmarked(const QString &url);
    static void insertBookmark(const BookmarkItem &item);
    static void removeBookmark(const QString &url);
    static QWidget *popupWidget(const BookmarkItem &item);

    static QStringList s_folderNames;

private:
    static QDomDocument s_xmlDom;

    QWidget *m_widget = nullptr;
    QTreeWidget *m_treeWidget = nullptr;
    QComboBox *m_folderBox = nullptr;
    QToolButton *m_addFolderButton = nullptr;
    QToolButton *m_removeFolderButton = nullptr;
    QToolButton *m_refreshButton = nullptr;

    void readBookmarksFile();

    void setupBookmarksWidget();
    void showFolderItems();
};
