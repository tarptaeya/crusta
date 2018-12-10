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
#include "sidebarbutton.h"

SideBarButton::SideBarButton(QWidget *parent)
    : QPushButton(parent)
{
    setObjectName("sidebar-button");
    m_webview = new QWebEngineView(this);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &SideBarButton::customContextMenuRequested, this, &SideBarButton::handleContextMenu);
}

QWebEngineView *SideBarButton::webView()
{
    return m_webview;
}

QString SideBarButton::baseUrl() const
{
    return m_baseUrlString;
}

void SideBarButton::setBaseUrl(const QString &urlString)
{
    m_baseUrlString = urlString;
}

void SideBarButton::handleContextMenu(const QPoint &pos)
{
    if (m_baseUrlString.isEmpty()) {
        return;
    }

    QMenu *menu = new QMenu;
    QAction *reloadAction = new QAction("Reload");
    QAction *unloadAction = new QAction("Unload");
    QAction *removeAction = new QAction("Remove");
    connect(reloadAction, &QAction::triggered, this, [this] {
        m_webview->load(m_baseUrlString);
    });
    connect(unloadAction, &QAction::triggered, this, [this] {
        m_webview->load(QUrl(""));
    });
    connect(removeAction, &QAction::triggered, this, [this] {
        emit removePanelRequested();
    });
    menu->addAction(reloadAction);
    menu->addAction(unloadAction);
    menu->addSeparator();
    menu->addAction(removeAction);
    menu->exec(mapToGlobal(pos));
    menu->deleteLater();
}
