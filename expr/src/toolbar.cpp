#include "toolbar.h"

ToolBar::ToolBar(QWidget *parent)
    : QToolBar (parent)
{
    m_backButton = new QToolButton;
    m_forwardButton = new QToolButton;
    m_refreshButton = new QToolButton;
    m_homeButton = new QToolButton;
    m_bookmarkButton = new QToolButton;
    m_menuButton = new QToolButton;
    m_addressBar = new QLineEdit;

    m_backButton->setIcon(QIcon::fromTheme(QStringLiteral("go-previous")));
    m_forwardButton->setIcon(QIcon::fromTheme(QStringLiteral("go-next")));
    m_refreshButton->setIcon(QIcon::fromTheme(QStringLiteral("")));
    m_homeButton->setIcon(QIcon::fromTheme(QStringLiteral("go-home")));

    addWidget(m_backButton);
    addWidget(m_forwardButton);
    addWidget(m_refreshButton);
    addWidget(m_homeButton);
    addWidget(m_addressBar);
    addWidget(m_bookmarkButton);
    addSeparator();
    addWidget(m_menuButton);

    setMovable(false);
}
