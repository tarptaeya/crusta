#include "bookmarks.h"
#include "utils.h"

#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlError>
#include <QSqlQuery>

#include <iostream>

/*
 * TODO: currently there are multiple calls to Bookmarks::isBookmarked,
 *       please try to reduce them.
 */


Bookmarks::Bookmarks(QObject *parent)
    : QObject (parent)
{
}

bool Bookmarks::isBookmarked(const QString &url)
{
    QSqlQuery sql;
    sql.prepare(QStringLiteral("select count(*) from bookmarks where url = ?"));
    sql.addBindValue(url);
    if (!sql.exec()) {
        std::cerr << "unable to check if " << url.toStdString() << " is bookmarked: " << sql.lastError().text().toStdString() << std::endl;
        return false;
    }
    sql.next();
    if (sql.value(0).toInt()) {
        return true;
    }

    return false;
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

QWidget *Bookmarks::popupWidget(const QString &titleText, const QString &url)
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

    title->setText(titleText);
    title->setCursorPosition(0);

    connect(remove, &QPushButton::clicked, [url, widget] {
        Bookmarks::removeBookmark(url);
        widget->close();
    });

    connect(save, &QPushButton::clicked, [title, description, url, folder, widget] {
        QSqlQuery sql;
        sql.prepare(QStringLiteral("update bookmarks set title = ?, description = ?, folder = ? where url = ?"));
        sql.addBindValue(title->text());
        sql.addBindValue(description->text());
        sql.addBindValue(folder->currentText());
        sql.addBindValue(url);
        if (!sql.exec()) {
            std::cerr << "unable to update bookmark: " << sql.lastError().text().toStdString() << std::endl;
        }

        widget->close();
    });

    return widget;
}
