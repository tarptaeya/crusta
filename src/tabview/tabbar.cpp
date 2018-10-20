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
#include "tabbar.h"
#include "tab.h"
#include "addtabbutton.h"
#include "webview.h"
#include "appmanager.h"

#define MAX_TAB_WIDTH 300
#define MIN_TAB_WIDTH 50
#define AUX_WIDTH 32

TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
    m_addTabButton = new AddTabButton(this);

    setAttribute(Qt::WA_StyledBackground);
    setDocumentMode(true);
    setElideMode(Qt::ElideRight);
    setMovable(true);
    setUsesScrollButtons(true);

    connect(m_addTabButton, &AddTabButton::clicked, this, []{
        Tab *tab = new Tab;
        WebView *webView = new WebView;
        tab->setWebView(webView);
        appManager->addTab(tab, Tab::Active);
    });
}

QSize TabBar::tabSizeHint(int index) const
{
    int count = this->count() ? this->count() : 1;
    int width = (this->size().width() - AUX_WIDTH) / count;
    if (width > MAX_TAB_WIDTH) {
        width = MAX_TAB_WIDTH;
    }
    if (width < MIN_TAB_WIDTH) {
        width = MIN_TAB_WIDTH;
    }

    QSize size = QTabBar::tabSizeHint(index);
    size.setWidth(width);

    if (index == count - 1) {
        updateAddTabButton(width);
    }

    return size;
}

void TabBar::tabInserted(int index)
{
    addTabCloseButton(index);
}

void TabBar::updateAddTabButton(int tabWidth) const
{
    // from stylesheet ->
    // width = height = 20px
    // height of tabbar = 32px;
    // so, margin = (32 - 20) / 2 => 6;
    int margin = 6;
    int xPos = tabWidth * count() + margin;
    m_addTabButton->move(xPos, margin);
}

void TabBar::addTabCloseButton(int index)
{
    QPushButton *tabCloseButton = new QPushButton;
    tabCloseButton->setObjectName("tab-close-button");
    tabCloseButton->setText("\u2a09");
    setTabButton(index, QTabBar::RightSide, tabCloseButton);

    connect(tabCloseButton, &QPushButton::clicked, this, [this, index]{
        tabCloseRequested(index);
    });
}
