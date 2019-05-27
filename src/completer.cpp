#include "completer.h"

#include "bookmarks.h"
#include "history.h"
#include <QCompleter>

Completer::Completer(QObject *parent)
    : QObject (parent)
{
}

void Completer::attachCompleterTo(QLineEdit *lineEdit)
{
    QStringList suggestions;

    for (const QString &address : Bookmarks::s_bookmarks.keys()) {
        suggestions << address;
    }

    for (const HistoryItem &item : History::getAllHistory()) {
        suggestions << item.url;
    }

    QCompleter *completer = new QCompleter(suggestions, lineEdit);
    completer->setFilterMode(Qt::MatchContains);
    lineEdit->setCompleter(completer);
}
