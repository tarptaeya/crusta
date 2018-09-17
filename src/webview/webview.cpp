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
#include "webview.h"
#include "webpage.h"
#include "../tabview/tabwidget.h"
#include "../bootstrap/appmanager.h"
#include "../bootstrap/settings.h"
#include "../bootstrap/database.h"
#include "../utils/dimensions.h"
#include "../utils/strings.h"
#include "../data/historyitem.h"
#include "../data/speeddialitem.h"
#include "../pages/speeddial/speeddial.h"
#include <QUrl>
#include <QMenu>
#include <QAction>
#include <QPoint>
#include <QWebEngineHistory>
#include <QApplication>
#include <QClipboard>
#include <QBuffer>
#include <QPixmap>

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
    m_webPage = new WebPage(appManager->webEngineProfile());
    setPage(m_webPage);
    setFocus();

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &WebView::customContextMenuRequested, this, &WebView::showContextMenu);

    connect(this, &WebView::loadStarted, this, &WebView::handleLoadStarted);
    connect(this, &WebView::loadFinished, this, &WebView::handleLoadFinished);
    connect(m_webPage, &WebPage::linkHovered, this, &WebView::handleLinkHovered);
}

WebView::~WebView()
{
    m_webPage->deleteLater();
}

void WebView::loadStartupUrl()
{
    appManager->settings()->beginGroup(WEBENGINE);
    const QUrl url = appManager->settings()->value(QStringLiteral("startupUrl"), QUrl("qrc:/html/speeddial/index.html")).toUrl();
    appManager->settings()->endGroup();
    load(url);
}

void WebView::setVirtualTabWidget(TabWidget *tabWidget)
{
    m_tabWidget = tabWidget;
}

void WebView::search(const QString &text)
{
    // TODO
    load(QUrl::fromUserInput(text));
}

int WebView::loadingTime() const
{
    return m_loadingTime;
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    WebView *webview = new WebView;
    switch (type) {
    case QWebEnginePage::WebBrowserTab:
        m_tabWidget->addTab(webview);
        break;
    case QWebEnginePage::WebBrowserBackgroundTab:
        m_tabWidget->addBackgroundTab(webview);
        break;
    case QWebEnginePage::WebBrowserWindow:

        break;
    case QWebEnginePage::WebDialog:

        break;
    }
    return webview;
}

void WebView::handleLoadStarted()
{
    m_isLoading = true;
    m_loadingTime = QTime::currentTime().msecsSinceStartOfDay();
}

void WebView::handleLoadFinished()
{
    m_isLoading = false;
    m_loadingTime = QTime::currentTime().msecsSinceStartOfDay() - m_loadingTime;

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    icon().pixmap(16, 16).save(&buffer, "PNG");

    HistoryItem item;
    item.setTimestamp(QDateTime::currentDateTime().toSecsSinceEpoch());
    item.setFavicon(byteArray);
    item.setTitle(title());
    item.setUrl(url().toString());
    item.setLoadingTime(m_loadingTime);

    appManager->database()->addHistoryEntry(item);

    const QList<SpeeddialItem> speeddialItems = Speeddial::speeddialItems();
    for (SpeeddialItem item : speeddialItems) {
        QUrl itemUrl = QUrl::fromEncoded(item.url().toUtf8());
        if (matchesCurrentUrl(itemUrl)) {
            item.setTitle(title());
            item.setImage(grabAsByteArray());
            appManager->database()->addSpeeddialEntry(item);
            break;
        }
    }
}

void WebView::handleLinkHovered(const QString &url)
{
    m_hoveredLink = url;
}

void WebView::showContextMenu(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);

    if (!m_hoveredLink.isEmpty()) {
        QAction *openInNewTabAction = menu->addAction(tr("Open link in new tab"));
        connect(openInNewTabAction, &QAction::triggered, this, [this]{
            m_webPage->triggerAction(WebPage::OpenLinkInNewTab);
        });
        QAction *openInNewBackgroundTabAction = menu->addAction(tr("Open link in new background tab"));
        connect(openInNewBackgroundTabAction, &QAction::triggered, this, [this]{
            m_webPage->triggerAction(WebPage::OpenLinkInNewBackgroundTab);
        });
        QAction *copyLinkURLAction = menu->addAction(tr("Copy link address"));
        connect(copyLinkURLAction, &QAction::triggered, this, [this]{
            m_webPage->triggerAction(WebPage::CopyLinkToClipboard);
        });
        menu->addSeparator();
    }

    QString text = selectedText();
    QString elidedText = Strings::elideString(text, Dimensions::contextMenuTextLength());
    if (!text.isEmpty()) {
        QAction *selectedTextCopyAction = menu->addAction(tr("Copy \"%1\"").arg(elidedText));
        connect(selectedTextCopyAction, &QAction::triggered, this, [text]{
            qApp->clipboard()->setText(text);
        });
        QAction *selectedTextSearchAction = menu->addAction(tr("Search \"%1\"").arg(elidedText));
        menu->addSeparator();
    }

    QAction *backAction = menu->addAction(tr("Back"));
    backAction->setEnabled(history()->canGoBack());
    connect(backAction, &QAction::triggered, this, &WebView::back);
    QAction *forwardAction = menu->addAction(tr("Forward"));
    forwardAction->setEnabled(history()->canGoForward());
    connect(forwardAction, &QAction::triggered, this, &WebView::forward);
    QAction *stopReloadAction = menu->addAction(m_isLoading ? tr("Stop") : tr("Reload"));
    connect(stopReloadAction, &QAction::triggered, this, [this]{
        if (m_isLoading) {
            stop();
        } else {
            reload();
        }
    });
    menu->addSeparator();

    QAction *saveAction = menu->addAction(tr("Save page"));
    connect(saveAction, &QAction::triggered, this, [this]{
        m_webPage->triggerAction(WebPage::SavePage);
    });
    menu->addSeparator();

    QAction *viewPageSourceAction = menu->addAction(tr("View page source"));
    connect(viewPageSourceAction, &QAction::triggered, this, [this]{
        m_webPage->triggerAction(WebPage::ViewSource);
    });
    QAction *inspectElementAction = menu->addAction(tr("Inspect element"));
    connect(inspectElementAction, &QAction::triggered, this, [this]{
        m_webPage->triggerAction(WebPage::InspectElement);
    });

    menu->exec(mapToGlobal(pos));
}

QByteArray WebView::grabAsByteArray()
{
    QPixmap pixmap = grab();
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    return byteArray;
}

bool WebView::matchesCurrentUrl(const QUrl &otherUrl)
{
    return otherUrl.adjusted(QUrl::RemoveScheme).host() == url().adjusted(QUrl::RemoveScheme).host();
}
