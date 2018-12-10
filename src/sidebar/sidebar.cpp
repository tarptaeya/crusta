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
#include "database.h"
#include "tab.h"
#include "webview.h"
#include "sidebarpopup.h"
#include "panelitem.h"
#include "tools.h"
#include <QPixmap>

#define QSL QStringLiteral

SideBar::SideBar(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setObjectName(QSL("sidebar"));

    m_vbox = new QVBoxLayout();
    m_vbox->setContentsMargins(0, 0, 0, 0);
    m_vbox->setSpacing(0);

    m_hbox = new QHBoxLayout();
    m_hbox->setContentsMargins(0, 0, 0, 0);
    m_hbox->setSpacing(0);
    m_hbox->addLayout(m_vbox);
    setLayout(m_hbox);

    SideBarButton *bookmarksButton = new SideBarButton(this);
    SideBarButton *downloadsButton = new SideBarButton(this);
    SideBarButton *historyButton = new SideBarButton(this);
    bookmarksButton->setIcon(QIcon(":/icons/favourite.svg"));
    downloadsButton->setIcon(QIcon(":/icons/download.svg"));
    historyButton->setIcon(QIcon(":/icons/history.svg"));
    m_vbox->addWidget(bookmarksButton);
    m_vbox->addWidget(downloadsButton);
    m_vbox->addWidget(historyButton);

    m_addPanelButton = new SideBarButton(this);
    m_addPanelButton->setIcon(QIcon(":/icons/plus.svg"));
    m_vbox->addWidget(m_addPanelButton);

    m_vbox->addWidget(new QWidget(this));

    const QList<PanelItem> panelItems = appManager->database()->loadPanels();
    for (const PanelItem &item : panelItems) {
        QPixmap pixmap;
        pixmap.loadFromData(item.favicon());
        QIcon icon = QIcon(pixmap);
        addPanel(item.url(), icon);
    }

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

    connect(m_addPanelButton, &SideBarButton::clicked, this, &SideBar::handleAddPanel);
}

SideBar::~SideBar()
{
}

void SideBar::handleAddPanel()
{
    SideBarPopup *popup = new SideBarPopup;
    popup->setAttribute(Qt::WA_DeleteOnClose);
    popup->setWindowFlag(Qt::Popup);
    popup->show();
    popup->move(m_addPanelButton->mapToGlobal(m_addPanelButton->rect().center()));

    connect(popup, &SideBarPopup::addPanelRequested, this, [this](const QString &urlString) {
        addPanel(urlString, QIcon());
        PanelItem item;
        item.setUrl(urlString);
        appManager->database()->addPanel(item);
    });
}

void SideBar::addPanel(const QString &urlString, const QIcon &icon)
{
    SideBarButton *button = new SideBarButton;
    button->setBaseUrl(urlString);
    button->setIcon(icon);
    m_vbox->insertWidget(m_vbox->count() - 2, button);
    connect(button, &QPushButton::clicked, this, [this, button] {
        showPanel(button);
    });

    connect(button->webView(), &QWebEngineView::iconChanged, button, [button](const QIcon &icon) {
        button->setIcon(icon);
        PanelItem item;
        item.setUrl(button->baseUrl());
        item.setFavicon(convertIconToByteArray(icon));
        appManager->database()->addPanel(item);
    });

    connect(button, &SideBarButton::removePanelRequested, this, [this, button] {
        removePanel(button->baseUrl());
        QWidget *widget = m_hbox->itemAt(1)->widget();
        if (button->webView() == widget) {
            m_hbox->removeItem(m_hbox->itemAt(1));
            widget->hide();
            resize(40, height());
            return;
        }
        button->deleteLater();
    });
}

void SideBar::showPanel(SideBarButton *button)
{
    if (m_hbox->count() == 2) {
        QWidget *widget = m_hbox->itemAt(1)->widget();
        widget->hide();
        m_hbox->removeItem(m_hbox->itemAt(1));

        if (button->webView() == widget) {
            resize(40, height());
            return;
        }
    }

    button->webView()->show();
    m_hbox->addWidget(button->webView(), 1);
    if (button->webView()->url().isEmpty()) {
        button->webView()->load(button->baseUrl());
    }
}

void SideBar::removePanel(const QString &urlString)
{
    appManager->database()->removePanel(urlString);
}
