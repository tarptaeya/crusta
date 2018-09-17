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
#include "tabbarbutton.h"
#include "tab.h"
#include "tabwidget.h"
#include "omnibar/omnibar.h"
#include "../utils/dimensions.h"
#include "../webview/webview.h"

TabBar::TabBar(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setFixedHeight(Dimensions::tabBarHeight());

    m_tabWidget = qobject_cast<TabWidget *>(parent);
    Q_ASSERT(m_tabWidget != nullptr);

    m_hBoxLayout = new QHBoxLayout(this);
    setLayout(m_hBoxLayout);
    m_hBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_hBoxLayout->setSpacing(0);

    m_backButton = new TabBarButton(this);
    m_forwardButton = new TabBarButton(this);
    m_stopReloadButton = new TabBarButton(this);
    m_homeButton = new TabBarButton(this);
    m_loadingTimeLabel = new QClickableLabel(this);
    m_newTabButton = new TabBarButton(this);
    m_tabListButton = new TabBarButton(this);
    m_optionsButton = new TabBarButton(this);

    m_backButton->setIcon(QIcon(":/icons/back.svg"));
    m_forwardButton->setIcon(QIcon(":/icons/forward.svg"));
    m_stopReloadButton->setIcon(QIcon(":/icons/refresh.svg"));
    m_homeButton->setIcon(QIcon(":/icons/home.svg"));
    m_newTabButton->setIcon(QIcon(":/icons/add.svg"));
    m_tabListButton->setIcon(QIcon(":/icons/list.svg"));
    m_optionsButton->setIcon(QIcon(":/icons/options.svg"));

    m_omniBar = new OmniBar(this);
    m_loadingTimeLabel->setMinimumWidth(height());
    m_loadingTimeLabel->setMargin(10);

    m_hBoxLayout->addWidget(m_backButton);
    m_hBoxLayout->addWidget(m_forwardButton);
    m_hBoxLayout->addWidget(m_stopReloadButton);
    m_hBoxLayout->addWidget(m_homeButton);
    m_hBoxLayout->addWidget(m_omniBar);
    m_hBoxLayout->addWidget(m_loadingTimeLabel);
    m_hBoxLayout->addWidget(m_newTabButton);
    m_hBoxLayout->addWidget(m_tabListButton);
    m_hBoxLayout->addWidget(m_optionsButton);

    connect(m_newTabButton, &TabBarButton::clicked, this, [this]{
        if (!m_tabWidget) {
            qWarning() << "Unable to cast parent to TabWidget *";
            return;
        }
        m_tabWidget->addTab();
    });

    connect(m_tabListButton, &TabBarButton::clicked, this, [this]{
        m_tabWidget->toggleTabList();
    });

    connect(m_omniBar, &OmniBar::returnPressed, this, [this]{
        if (!m_virtualTab) {
            return;
        }
        m_virtualTab->webview()->search(m_omniBar->text());
    });
}

void TabBar::setVirtualTab(Tab *tab)
{
    if (!tab) {
        return;
    }

    destroyVirtualConnections();
    m_virtualTab = tab;
    createVirtualConnections();
}

void TabBar::createVirtualConnections()
{
    const QMetaObject::Connection connection1 = connect(m_backButton, &TabBarButton::clicked, this, [this]{
        if (!m_virtualTab) {
            return;
        }
        m_virtualTab->webview()->back();
    });
    const QMetaObject::Connection connection2 = connect(m_forwardButton, &TabBarButton::clicked, this, [this]{
        if (!m_virtualTab) {
            return;
        }
        m_virtualTab->webview()->forward();
    });
    const QMetaObject::Connection connection3 = connect(m_virtualTab->webview(), &WebView::loadStarted, [this]{
        m_stopReloadButton->setIcon(QIcon(":/icons/close.svg"));
        m_stopReloadButton->setData(QVariant::fromValue(QStringLiteral("STOP")));
        m_loadingTimeLabel->setText("...");
    });
    const QMetaObject::Connection connection4 = connect(m_virtualTab->webview(), &WebView::loadFinished, [this]{
        m_stopReloadButton->setIcon(QIcon(":/icons/refresh.svg"));
        m_stopReloadButton->setData(QVariant::fromValue(QStringLiteral("RELOAD")));
        m_loadingTimeLabel->setText(QString("%1s").arg(QString::number(m_virtualTab->webview()->loadingTime() / 10 / 100.0)));
    });
    const QMetaObject::Connection connection5 = connect(m_stopReloadButton, &TabBarButton::clicked, [this]{
        if (!m_virtualTab) {
            return;
        }
        if (m_stopReloadButton->data().toString() == QStringLiteral("STOP")) {
            m_virtualTab->webview()->stop();
        } else {
            m_virtualTab->webview()->reload();
        }
    });
    const QMetaObject::Connection connection6 = connect(m_virtualTab->webview(), &WebView::urlChanged, [this](const QUrl &url){
        m_omniBar->setUrl(url);
    });

    m_connections.append(connection1);
    m_connections.append(connection2);
    m_connections.append(connection3);
    m_connections.append(connection4);
    m_connections.append(connection5);
    m_connections.append(connection6);
}

void TabBar::destroyVirtualConnections()
{
    while (m_connections.length() > 0) {
        const QMetaObject::Connection connection = m_connections.front();
        QObject::disconnect(connection);
        m_connections.pop_front();
    }
}
