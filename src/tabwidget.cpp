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
    m_newTabButton->setIcon(QIcon::fromTheme("list-add"));
    m_newTabButton->setAutoRaise(true);
    setCornerWidget(m_newTabButton);
    connect(m_newTabButton, &QToolButton::clicked, this, &TabWidget::newTab);

    setContentsMargins(0, 0, 0, 0);

    newTab();

    connect(this, &TabWidget::tabCloseRequested, this, &TabWidget::closeTab);
}

Tab *TabWidget::newTab()
{
    Tab *tab = new Tab;
    addTab(tab, tr("New Tab"));

    return tab;
}

void TabWidget::closeTab(int index)
{
    Tab *tab = dynamic_cast<Tab *>(widget(index));
    if (!tab) return;

    removeTab(index);
    tab->deleteLater();
}
