#include "omnibar.h"

OmniBar::OmniBar(QWidget *parent)
    : QLineEdit(parent)
{
    setObjectName("omni-bar");
    setAttribute(Qt::WA_MacShowFocusRect, false);

    m_siteInfoAction = new QAction(this);
    m_siteInfoAction->setIcon(QIcon(":/icons/globe.svg"));
    addAction(m_siteInfoAction, QLineEdit::LeadingPosition);

    m_bookmarkPageAction = new QAction(this);
    m_bookmarkPageAction->setIcon(QIcon(":/icons/favourite.svg"));
    addAction(m_bookmarkPageAction, QLineEdit::TrailingPosition);
}
