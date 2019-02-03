#ifndef CR_DATABASE_H
#define CR_DATABASE_H

#include "bookmarkitem.h"
#include "historyitem.h"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class DataBase : public QObject
{
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void createTables();
    void addHistory(const HistoryItem &item);
    void removeHistory(const QString &address);
    void removeAllHistory();
    QList<HistoryItem> history() const;

    void addBookmark(const BookmarkItem &item);
    void removeBookmark(const QString &address);
    void updateBookmark(const BookmarkItem &item);
    QList<BookmarkItem> bookmarks(const QString &folderName) const;
    QStringList bookmarkFolders() const;
    BookmarkItem isBookmarked(const QString &address) const;

private:
    QSqlDatabase m_db;
    QString m_name;
};

#endif
