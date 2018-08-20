#include "tablist.h"
#include "tablistitem.h"
#include "tab.h"
#include "../bootstrap/appmanager.h"
#include "../bootstrap/settings.h"
#include "../utils/dimensions.h"
#include <QScrollArea>

TabList::TabList(QWidget *parent)
    : QWidget(parent)
{
    appManager->settings()->beginGroup(APPEARANCE);
    bool isTabListVisible = appManager->settings()->value(QStringLiteral("tabListVisible"), false).toBool();
    m_preferedWidth = appManager->settings()->value(QStringLiteral("tabListWidth"), Dimensions::tabListWidth()).toInt();
    appManager->settings()->endGroup();

    if (!isTabListVisible) {
        hide();
    }

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->setContentsMargins(2, 0, 2, 2);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Light);
    QWidget *widget = new QWidget(scrollArea);
    scrollArea->setWidget(widget);
    scrollArea->setWidgetResizable(true);
    m_vBoxLayout = new QVBoxLayout;
    m_vBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_vBoxLayout->setSpacing(0);
    widget->setLayout(m_vBoxLayout);
    vBoxLayout->addWidget(scrollArea);
    setLayout(vBoxLayout);

    QWidget *paddingWidget = new QWidget(widget);
    m_vBoxLayout->addWidget(paddingWidget);
}

void TabList::show()
{
    resize(m_preferedWidth, height());
    QWidget::show();
}

void TabList::addTab(Tab *tab)
{
    TabListItem *tabListItem = new TabListItem;
    tabListItem->setVirtualTab(tab);
    tabListItem->setVirtualTabWidget(m_tabWidget);
    m_vBoxLayout->insertWidget(0, tabListItem);
    m_tabCount++;
}

void TabList::closeTab(Tab *tab)
{
    m_vBoxLayout->removeWidget(tab->tabListItem());
    tab->tabListItem()->deleteLater();
    m_tabCount--;
}

void TabList::setVirtualTabWidget(TabWidget *tabWidget)
{
    m_tabWidget = tabWidget;
}

int TabList::indexOf(Tab *tab)
{
    int index = m_vBoxLayout->indexOf(tab->tabListItem());
    return m_tabCount - index - 1;
}
