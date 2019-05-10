#pragma once

#include <QIcon>
#include <QObject>
#include <QWidget>

struct BookmarkItem
{
    QIcon icon;
    QString title;
    QString url;
    QString description;
    QString folder;
};

class Bookmarks : public QObject
{
public:
    explicit Bookmarks(QObject *parent = nullptr);

    static bool isBookmarked(const QString &url);
    static void insertBookmark(const BookmarkItem &item);
    static void removeBookmark(const QString &url);
    static QWidget *popupWidget(const QString &titleText, const QString &url);
};
