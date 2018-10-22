#include "omnibar.h"
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
