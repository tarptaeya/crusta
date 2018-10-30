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
#include "tabwidget.h"
#include "tab.h"
#include "tabbar.h"
#include "webview.h"

#include <QMenu>
#include <QAction>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    m_tabBar = new TabBar(this);
    setTabBar(m_tabBar);

    Tab *tab = new Tab;
    WebView *webView = new WebView;
    webView->load(QUrl("https://google.com"));
    tab->setWebView(webView);
    addTab(tab, Tab::Active);

    connect(m_tabBar, &TabBar::customContextMenuRequested, this, &TabWidget::showTabBarContextMenu);
    connect(m_tabBar, &TabBar::tabCloseRequested, this, &TabWidget::closeTab);
}

int TabWidget::addTab(Tab *tab, int type)
{
    int index = QTabWidget::addTab(tab, tab->icon(), tab->title());
    if (type == Tab::Active) {
        setCurrentIndex(index);
    }
    return index;
}

void TabWidget::closeTab(int index)
{
    Tab *tab = getTab(index);
    removeTab(index);
    tab->deleteLater();
}

Tab *TabWidget::getTab(int index)
{
    return qobject_cast<Tab *>(widget(index));
}

void TabWidget::showTabBarContextMenu(const QPoint &pos)
{
    int tabIndex = m_tabBar->tabAt(pos);
    Tab *tab = getTab(tabIndex);

    if (!tab) {
        return;
    }

    QMenu *menu = new QMenu(this);
    QAction *newTabAction = new QAction(tr("New tab"));

    QAction *reloadAction = new QAction(tr("Reload tab"));
    QAction *duplicateAction = new QAction(tr("Duplicate tab"));
    QAction *muteUnmuteAction = new QAction();
    if (tab->webView()->page()->isAudioMuted()) {
        muteUnmuteAction->setText(tr("Unmute tab"));
    } else {
        muteUnmuteAction->setText(tr("Mute tab"));
    }

    QAction *closeAction = new QAction(tr("Close tab"));
    QAction *closeOtherAction = new QAction(tr("Close other tabs"));
    QAction *closeLeftAction = new QAction(tr("Close tabs to the left"));
    QAction *closeRightAction = new QAction(tr("Close tabs to the right"));

    connect(newTabAction, &QAction::triggered, m_tabBar, &TabBar::addNewTab);
    connect(reloadAction, &QAction::triggered, tab->webView(), &WebView::reload);
    // TODO: connect(duplicateAction)
    connect(muteUnmuteAction, &QAction::triggered, this, [tab]{
        tab->webView()->page()->setAudioMuted(!tab->webView()->page()->isAudioMuted());
    });
    connect(closeAction, &QAction::triggered, this, [this, tabIndex] {
        closeTab(tabIndex);
    });
    connect(closeOtherAction, &QAction::triggered, this, [this, tabIndex] {
        int totalTabs = m_tabBar->count();
        for (int index = tabIndex + 1; index < totalTabs; index++) {
            closeTab(tabIndex + 1);
        }
        for (int index = 0; index < tabIndex; index++) {
            closeTab(0);
        }
    });
    connect(closeLeftAction, &QAction::triggered, this, [this, tabIndex] {
        for (int index = 0; index < tabIndex; index++) {
            closeTab(0);
        }
    });
    connect(closeRightAction, &QAction::triggered, this, [this, tabIndex] {
        int totalTabs = m_tabBar->count();
        for (int index = tabIndex + 1; index < totalTabs; index++) {
            closeTab(tabIndex + 1);
        }
    });

    menu->addAction(newTabAction);
    menu->addSeparator();
    menu->addAction(reloadAction);
    menu->addAction(duplicateAction);
    menu->addAction(muteUnmuteAction);
    menu->addSeparator();
    menu->addAction(closeAction);
    menu->addAction(closeOtherAction);
    menu->addAction(closeLeftAction);
    menu->addAction(closeRightAction);
    menu->exec(m_tabBar->mapToGlobal(pos));
}
