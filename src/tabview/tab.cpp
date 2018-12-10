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
#include "bookmarksbar.h"
#include "infobar.h"
#include "toolbar.h"
#include "tabwidget.h"
#include "webview.h"
#include "appmanager.h"
#include "searchenginemanager.h"
#include "bookmarksmanager.h"
#include "historymanager.h"

#include <QWebEngineHistory>

Tab::Tab(QWidget *parent)
    : QWidget(parent)
{
    m_vBoxLayout = new QVBoxLayout(this);
    m_vBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_vBoxLayout->setSpacing(0);
    setLayout(m_vBoxLayout);

    m_toolBar = new ToolBar(this);
    m_vBoxLayout->addWidget(m_toolBar, 0);

    m_baseWidget = new QWidget(this);
    m_baseWidget->setObjectName("base-widget");
    m_baseWidget->setStyleSheet("#base-widget{ background-color: white }");
    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->addWidget(m_baseWidget);
    m_vBoxLayout->addWidget(m_stackedWidget, 1);

    connect(m_toolBar, &ToolBar::backRequested, this, [this]{
        if (!m_webView) {
            return;
        }
        m_webView->back();
    });
    connect(m_toolBar, &ToolBar::forwardRequested, this, [this]{
        if (!m_webView) {
            return;
        }
        m_webView->forward();
    });
    connect(m_toolBar, &ToolBar::stopReloadRequested, this, [this]{
        if (!m_webView) {
            return;
        }
        if (m_webView->isLoading()) {
            m_webView->stop();
        } else {
            m_webView->reload();
        }
    });
    connect(m_toolBar, &ToolBar::navigationRequested, this, [this](const QString &text) {
        if (!m_webView) {
            return;
        }
        QUrl url = QUrl::fromUserInput(text);
        if (!url.isValid() || text.count(':') == 0) {
            url = SearchEngineManager::getSearchUrl(text);
        }
        m_webView->load(url);
        m_stackedWidget->setCurrentIndex(1);
    });
}

QString Tab::title() const
{
    if (!m_webView) {
        return QString();
    }
    return m_webView->title();
}

QString Tab::urlString() const
{
    if (!m_webView) {
        return QString();
    }
    return QString::fromUtf8(m_webView->url().toEncoded());
}

QIcon Tab::icon() const
{
    if (!m_webView) {
        return QIcon();
    }
    return m_webView->icon();
}

WebView *Tab::webView()
{
    return m_webView;
}

void Tab::setWebView(WebView *webView)
{
    if (m_webView) {
        return;
    }
    m_webView = webView;
    m_stackedWidget->addWidget(m_webView);
    m_stackedWidget->setCurrentIndex(1);

    connect(m_webView, &WebView::titleChanged, this, [this](const QString &title){
        TabWidget *tabWidget = appManager->getTabWidget(this);
        tabWidget->setTabText(tabWidget->indexOf(this), title);
    });

    connect(m_webView, &WebView::iconChanged, this, [this](const QIcon &icon){
        TabWidget *tabWidget = appManager->getTabWidget(this);
        tabWidget->setTabIcon(tabWidget->indexOf(this), icon);
    });

    connect(m_webView, &WebView::loadStarted, this, [this]{
        m_toolBar->setAddress(m_webView->url());
        m_toolBar->setIsLoading(true);
    });

    connect(m_webView, &WebView::loadFinished, this, [this]{
        m_toolBar->setIsCanGoBack(m_webView->history()->canGoBack());
        m_toolBar->setIsCanGoForward(m_webView->history()->canGoForward());
        m_toolBar->setIsLoading(false);
    });

    connect(m_webView, &WebView::urlChanged, m_toolBar, &ToolBar::setAddress);

    connect(m_webView, &WebView::loadProgress, m_toolBar, &ToolBar::setProgress);
}

void Tab::showBookmarksManager()
{
    BookmarksManager::showBookmarksManager(m_baseWidget);
    m_stackedWidget->setCurrentIndex(0);
}

void Tab::showHistoryManager()
{
    HistoryManager::showHistoryManager(m_baseWidget);
    m_stackedWidget->setCurrentIndex(0);
}

int Tab::index()
{
    return appManager->indexOfTab(this);
}
