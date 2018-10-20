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
#include "toolbar.h"
#include "omnibar.h"
#include "toolbarbutton.h"

#define SPACER_WIDGET_WIDTH 75

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("tool-bar");

    m_hBoxLayout = new QHBoxLayout(this);
    m_hBoxLayout->setContentsMargins(5, 5, 5, 5);
    m_hBoxLayout->setSpacing(0);
    setLayout(m_hBoxLayout);

    m_backButton = new ToolBarButton(this);
    m_backButton->setIconFromFileName(":/icons/back.svg");
    m_forwardButton = new ToolBarButton(this);
    m_forwardButton->setIconFromFileName(":/icons/forward.svg");
    m_stopReloadButton = new ToolBarButton(this);
    m_favouritesButton = new ToolBarButton(this);
    m_favouritesButton->setIconFromFileName(":/icons/widgets.svg");
    m_omniBar = new OmniBar(this);
    m_shieldButton = new ToolBarButton(this);
    m_shieldButton->setIconFromFileName(":/icons/shield.svg");
    m_downloadsButton = new ToolBarButton(this);
    m_downloadsButton->setIconFromFileName(":/icons/download.svg");

    m_hBoxLayout->addWidget(m_backButton);
    m_hBoxLayout->addWidget(m_forwardButton);
    m_hBoxLayout->addWidget(m_stopReloadButton);
    m_hBoxLayout->addWidget(m_favouritesButton);
    m_hBoxLayout->addWidget(spacerWidget(SPACER_WIDGET_WIDTH));
    m_hBoxLayout->addWidget(m_omniBar);
    m_hBoxLayout->addWidget(spacerWidget(SPACER_WIDGET_WIDTH));
    m_hBoxLayout->addWidget(m_shieldButton);
    m_hBoxLayout->addWidget(m_downloadsButton);
}

QWidget *ToolBar::spacerWidget(int width)
{
    QWidget *widget = new QWidget(this);
    widget->setMinimumWidth(width);
    return widget;
}
