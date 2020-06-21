#include "tabwidget.h"
#include "tab.h"
#include <QTabBar>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setDocumentMode(true);
    setTabsClosable(true);
    setMovable(true);
    tabBar()->setExpanding(true);

    m_newTabButton = new QToolButton;
    m_newTabButton->setAutoRaise(true);
    setCornerWidget(m_newTabButton);
    connect(m_newTabButton, &QToolButton::clicked, this, &TabWidget::newTab);

    setContentsMargins(0, 0, 0, 0);

    newTab();
}

Tab *TabWidget::newTab()
{
    Tab *tab = new Tab;
    addTab(tab, tr("New Tab"));

    return tab;
}
