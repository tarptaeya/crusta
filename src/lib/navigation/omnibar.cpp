#include "common-defs.h"
#include "database.h"
#include "mainapplication.h"
#include "omnibar.h"
#include "searchenginemanager.h"

#include <QSet>

OmniBar::OmniBar(QWidget *parent)
    : QLineEdit (parent)
{
    m_siteInfoAction = new QAction(this);
    m_bookMarksAction = new QAction(this);

    m_completer = new QCompleter(this);
    m_model = new QStringListModel(this);
    m_completer->setModel(m_model);
    m_completer->setFilterMode(Qt::MatchContains);

    m_siteInfoAction->setIcon(QIcon::fromTheme(QSL("applications-internet")));
    m_bookMarksAction->setIcon(QIcon::fromTheme(QSL("")));
    m_siteInfoAction->setToolTip(QSL("Site Info"));
    m_bookMarksAction->setToolTip(QSL("Bookmark Page"));

    setCompleter(m_completer);

    addAction(m_siteInfoAction, OmniBar::LeadingPosition);
    addAction(m_bookMarksAction, OmniBar::TrailingPosition);

    loadCompleter();

    connect(this, &QLineEdit::returnPressed, this, &OmniBar::returnPressed);
}

void OmniBar::update(const QString &address)
{
    setText(address);
    setCursorPosition(0);

    if (address.startsWith(QSL("https://")) || address.startsWith(QSL("crusta://"))) {
        m_siteInfoAction->setIcon(QIcon::fromTheme(QSL("system-lock-screen")));
    } else {
        m_siteInfoAction->setIcon(QIcon::fromTheme(QSL("applications-internet")));
    }

    m_model->setStringList((m_model->stringList() << address).toSet().toList());
}

QAction *OmniBar::siteInfoAction()
{
    return m_siteInfoAction;
}

QAction *OmniBar::bookmarksAction()
{
    return m_bookMarksAction;
}

void OmniBar::returnPressed()
{
    QString text = this->text();
    if (text.contains(QL1C('.')) && !text.contains(QL1C(' '))) {
        if (!text.startsWith(QSL("http://"))
                && !text.startsWith(QSL("https://"))
                && !text.startsWith(QSL("crusta:"))) {
            text = text.prepend(QSL("http://"));
        }
    } else {
        text = appManager->searchEngineManager()->getQuery(text);
    }

    emit loadRequested(text);
}

void OmniBar::loadCompleter()
{
    QStringList list;
    const QList<HistoryItem> items = appManager->dataBase()->history();
    for (const HistoryItem &item : items) {
        list << item.url;
    }

    m_model->setStringList(list.toSet().toList());
}
