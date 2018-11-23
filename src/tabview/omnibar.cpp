#include "omnibar.h"
#include "siteinfopopup.h"
#include "bookmarkpopup.h"
#include "appmanager.h"
#include "database.h"
#include "bookmarksitem.h"
#include <QTimer>

#define QSL QStringLiteral

OmniBar::OmniBar(QWidget *parent)
    : QLineEdit(parent)
{
    setObjectName("omni-bar");
    setAttribute(Qt::WA_MacShowFocusRect, false);

    m_siteInfoAction = new QAction(this);
    m_siteInfoAction->setIcon(QIcon(QSL(":/icons/globe.svg")));
    addAction(m_siteInfoAction, QLineEdit::LeadingPosition);

    m_bookmarkPageAction = new QAction(this);
    m_bookmarkPageAction->setIcon(QIcon(QSL(":/icons/favourite.svg")));
    addAction(m_bookmarkPageAction, QLineEdit::TrailingPosition);

    connect(m_siteInfoAction, &QAction::triggered, this, [this] {
        SiteInfoPopup *siteInfoPopup = new SiteInfoPopup();
        siteInfoPopup->setParentAction(m_siteInfoAction);
        siteInfoPopup->setUrl(m_address);
        siteInfoPopup->show();
    });

    connect(m_bookmarkPageAction, &QAction::triggered, this, [this] {
        BookmarkPopup *bookmarkPopup = new BookmarkPopup();
        bookmarkPopup->setParentAction(m_bookmarkPageAction);
        bookmarkPopup->show();
    });
}

void OmniBar::setAddress(const QUrl &address)
{
    m_address = address;
    if (hasFocus()) {
        return;
    }
    setText(address.host());
    setCursorPosition(0);
    if (address.scheme() == QSL("https")) {
        m_siteInfoAction->setIcon(QIcon(QSL(":/icons/secure_globe.svg")));
    } else if (address.scheme() == QSL("http")) {
        m_siteInfoAction->setIcon(QIcon(QSL(":/icons/globe.svg")));
    }

    const QString urlString = QString::fromUtf8(address.toEncoded());
    if (!appManager->database()->isBookmarked(urlString).url().isEmpty()) {
        updateBookmarksIcon(true);
    } else {
        updateBookmarksIcon(false);
    }
}

void OmniBar::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);

    setAlignment(Qt::AlignLeft);
    setText(QString::fromUtf8(m_address.toEncoded()));
    QTimer::singleShot(0, this, [this]{
        setCursorPosition(text().length());
        cursorBackward(true, text().length());
    });
}

void OmniBar::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);

    setAlignment(Qt::AlignCenter);
    setAddress(m_address);
}

void OmniBar::updateBookmarksIcon(bool isBookmarked)
{
    if (isBookmarked) {
        m_bookmarkPageAction->setIcon(QIcon(QSL(":/icons/red_favourite.svg")));
    } else {
        m_bookmarkPageAction->setIcon(QIcon(QSL(":/icons/favourite.svg")));
    }
}
