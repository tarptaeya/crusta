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
#include "sidebar.h"
#include "strings.h"
#include "dimensions.h"
#include "sidebarbutton.h"
#include "appmanager.h"
#include "tab.h"
#include "webview.h"

#define QSL QStringLiteral

SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setObjectName(QSL("sidebar"));

    m_vbox = new QVBoxLayout();
    m_vbox->setContentsMargins(0, 0, 0, 0);
    m_vbox->setSpacing(0);
    setLayout(m_vbox);

    SideBarButton *bookmarksButton = new SideBarButton(this);
    SideBarButton *downloadsButton = new SideBarButton(this);
    SideBarButton *historyButton = new SideBarButton(this);
    bookmarksButton->setIcon(QIcon(":/icons/red_favourite.svg"));
    downloadsButton->setIcon(QIcon(":/icons/download.svg"));
    historyButton->setIcon(QIcon(":/icons/.svg"));
    m_vbox->addWidget(bookmarksButton);
    m_vbox->addWidget(downloadsButton);
    m_vbox->addWidget(historyButton);

    SideBarButton *addPanelButton = new SideBarButton(this);
    addPanelButton->setIcon(QIcon(":/icons/plus.svg"));
    m_vbox->addWidget(addPanelButton);

    m_vbox->addWidget(new QWidget(this));

    connect(bookmarksButton, &SideBarButton::clicked, this, [] {
        Tab *tab = new Tab;
        WebView *webView = new WebView;
        tab->setWebView(webView);
        tab->showBookmarksManager();

        appManager->addTab(tab, Tab::Active);
    });

    connect(historyButton, &SideBarButton::clicked, this, [] {
        Tab *tab = new Tab;
        WebView *webView = new WebView;
        tab->setWebView(webView);
        tab->showHistoryManager();

        appManager->addTab(tab, Tab::Active);
    });
}

SideBar::~SideBar()
{
}
