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
#pragma once

#include "qclickablelabel.h"
#include <QWidget>
#include <QHBoxLayout>

class TabWidget;
class TabBarButton;
class OmniBar;
class Tab;

class TabBar : public QWidget
{
public:
    explicit TabBar(QWidget *parent = nullptr);
    void setVirtualTab(Tab *tab);
private:
    TabWidget *m_tabWidget = nullptr;
    QHBoxLayout *m_hBoxLayout = nullptr;

    TabBarButton *m_backButton = nullptr;
    TabBarButton *m_forwardButton = nullptr;
    TabBarButton *m_stopReloadButton = nullptr;
    TabBarButton *m_homeButton = nullptr;
    TabBarButton *m_newTabButton = nullptr;
    TabBarButton *m_tabListButton = nullptr;
    QClickableLabel *m_loadingTimeLabel = nullptr;
    TabBarButton *m_optionsButton = nullptr;

    OmniBar *m_omniBar = nullptr;

    Tab *m_virtualTab = nullptr;

    QList<QMetaObject::Connection> m_connections;

    void createVirtualConnections();
    void destroyVirtualConnections();
};
