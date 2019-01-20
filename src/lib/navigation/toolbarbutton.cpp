#include "common-defs.h"
#include "toolbarbutton.h"

#include <QApplication>

ToolBarButton::ToolBarButton(QWidget *parent)
    : QToolButton (parent)
{
    setAutoRaise(true);

    setFixedSize(30, 30);
    setIconSize(QSize(24, 24));

    // hide the menu-indicator
    setStyleSheet(QSL("QToolButton::menu-indicator { image: none; }"));
}

void ToolBarButton::setBadgeCount(int count)
{
    if (!m_badgeLabel) {
        m_badgeLabel = new QLabel(this);
        m_badgeLabel->setAutoFillBackground(true);
        m_badgeLabel->setForegroundRole(QPalette::Light);
        m_badgeLabel->setBackgroundRole(QPalette::Text);
    }

    QString text = QSL(" ");
    if (count > 9) {
        text += QSL("9+");
    } else {
        text += QString::number(count);
    }
    text += QSL(" ");

    m_badgeLabel->setText(text);
    if (qApp->layoutDirection() == Qt::LeftToRight) {
        m_badgeLabel->move(width() - QFontMetrics(m_badgeLabel->font()).width(text), 0);
    }
}
