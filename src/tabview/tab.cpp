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
#include "tab.h"
#include "tabwidget.h"
#include "../webview/webview.h"
#include <QVBoxLayout>

Tab::Tab(QWidget *parent, WebView *webview)
    : QWidget(parent)
{
    m_webView = webview;
    if (!m_webView) {
        m_webView = new WebView(this);
    }

    QVBoxLayout *vBoxLayout = new QVBoxLayout(this);
    setLayout(vBoxLayout);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);
    vBoxLayout->addWidget(m_webView);
}

WebView *Tab::webview() const
{
    return m_webView;
}

TabListItem *Tab::tabListItem() const
{
    return m_tabListItem;
}

void Tab::setTabListItem(TabListItem *tabListItem)
{
    m_tabListItem = tabListItem;
}

void Tab::closeTab()
{
    deleteLater();
}

void Tab::setVirtualTabWidget(TabWidget *tabWidget)
{
    m_tabWidget = tabWidget;
    m_webView->setVirtualTabWidget(tabWidget);
}
