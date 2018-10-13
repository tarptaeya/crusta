/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <tarptaeya@gmail.com>
*
* THIS FILE IS A PART OF CRUSTA
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "tablist.h"
#include "tablistitem.h"
#include "tab.h"
#include "tabwidget.h"
#include "appmanager.h"
#include "settings.h"
#include "dimensions.h"
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

    QClickableLabel *paddingLabel = new QClickableLabel(widget);
    connect(paddingLabel, &QClickableLabel::doubleClicked, this, [this]{
        if (!m_tabWidget) {
            return;
        }
        m_tabWidget->addTab();
    });
    m_vBoxLayout->addWidget(paddingLabel);
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
    m_tabs.append(tab);
    m_tabCount++;
}

void TabList::closeTab(Tab *tab)
{
    m_vBoxLayout->removeWidget(tab->tabListItem());
    m_tabs.removeOne(tab);
    tab->tabListItem()->deleteLater();
    m_tabCount--;
}

void TabList::setVirtualTabWidget(TabWidget *tabWidget)
{
    m_tabWidget = tabWidget;
}

int TabList::indexOf(Tab *tab)
{
    return m_tabs.indexOf(tab);
}

Tab *TabList::tabAt(int index)
{
    return m_tabs.at(index);
}

QList<Tab *> TabList::tabs() const
{
    return m_tabs;
}
