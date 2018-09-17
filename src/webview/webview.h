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

#include <QWebEngineView>

class WebPage;
class TabWidget;

class WebView : public QWebEngineView
{
public:
    explicit WebView(QWidget *parent = nullptr);
    ~WebView();
    void loadStartupUrl();
    void setVirtualTabWidget(TabWidget *tabWidget);
    void search(const QString &text);
    bool isLoading() const;
    QString hoveredLink() const;
    int loadingTime() const;
protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type);
private:
    WebPage *m_webPage = nullptr;
    TabWidget *m_tabWidget = nullptr;
    bool m_isLoading = false;
    QString m_hoveredLink;
    int m_loadingTime;

    void handleLoadStarted();
    void handleLoadFinished();
    void handleLinkHovered(const QString &url);
    void showContextMenu(const QPoint &pos);
    QByteArray grabAsByteArray();
    bool matchesCurrentUrl(const QUrl &otherUrl);
};
