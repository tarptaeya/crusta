#include "common-defs.h"
#include "omnibar.h"

OmniBar::OmniBar(QWidget *parent)
    : QLineEdit (parent)
{
    m_siteInfoAction = new QAction(this);
    m_bookMarksAction = new QAction(this);

    m_siteInfoAction->setIcon(QIcon::fromTheme(QSL("applications-internet")));
    m_bookMarksAction->setIcon(QIcon::fromTheme(QSL("")));

    m_siteInfoAction->setToolTip(QSL("Site Info"));
    m_bookMarksAction->setToolTip(QSL("Bookmark Page"));

    addAction(m_siteInfoAction, OmniBar::LeadingPosition);
    addAction(m_bookMarksAction, OmniBar::TrailingPosition);

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
    QString address = text();
    emit loadRequested(address);
}
