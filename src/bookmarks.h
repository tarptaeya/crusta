#pragma once

#include <QComboBox>
#include <QIcon>
#include <QLineEdit>
#include <QObject>
#include <QToolButton>
#include <QTreeWidget>
#include <QWidget>

struct BookmarkItem
{
    QIcon icon;
    QString title;
    QString url;
    QString description;
    QString folder = QStringLiteral("General");
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
    QWidget *m_widget = nullptr;
    QTreeWidget *m_treeWidget = nullptr;
    QComboBox *m_folderBox = nullptr;
    QToolButton *m_addFolderButton = nullptr;
    QToolButton *m_removeFolderButton = nullptr;
    QToolButton *m_refreshButton = nullptr;

    void setupBookmarksWidget();
    void showFolderItems();
};
