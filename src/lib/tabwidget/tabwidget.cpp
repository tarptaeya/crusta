#include "common-defs.h"
#include "mainapplication.h"
#include "tab.h"
#include "tabbar.h"
#include "tabwidget.h"

#include <QPushButton>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget (parent)
{
    m_tabBar = new TabBar(this);

    setTabBar(m_tabBar);

    connect(this, &TabWidget::tabCloseRequested, this, &TabWidget::closeTab);
}

int TabWidget::addTab()
{
    const QString newTabUrl = appManager->settings()->value(QSL("webView/newTabUrl"), QSL("crusta://speeddial")).toString();
    return addTab(newTabUrl);
}

int TabWidget::addTab(const QString &address)
{
    Tab *tab = new Tab(address, this);
    return addTab(tab);
}

int TabWidget::addTab(Tab *tab)
{
    tab->setTabWidget(this);
    return QTabWidget::addTab(tab, tab->icon(), tab->title());
}

void TabWidget::closeTab(int index)
{
    QWidget *tab = widget(index);
    removeTab(index);
    tab->deleteLater();
}

Tab *TabWidget::currentTab()
{
    return static_cast<Tab *>(currentWidget());
}

Tab *TabWidget::tabAt(int index)
{
    return static_cast<Tab *>(widget(index));
}
