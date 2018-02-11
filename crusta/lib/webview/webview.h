/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <anmol@crustabrowser.com>
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

#ifndef WEBVIEW_H
#define WEBVIEW_H

#include "../utils/fullscreennotifier.h"
#include "../utils/timenotifier.h"
#include "webpage.h"
#include "../utils/crustaspeech.h"
#include "../managers/downloadwidget.h"

#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineDownloadItem>
#include <QString>
#include <QLayout>
#include <QAction>
#include <QStackedWidget>
#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QIcon>
#include <QTabBar>
#include <QPixmap>
#include <QUrl>
#include <QAuthenticator>
#include <QPoint>
#include <QSet>



class WebView:public QWebEngineView{
    Q_OBJECT
protected:
    QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);
public:
    bool favLoaded;
    int notif;
    int mouse;
    int geo;
    int audio;
    int video;
    int av;
    QString link="";
    WebPage* webpage=new WebPage();
    QString home_page;
    QString defaultSearch;
    QLayout* layout;
    QWidget* widget;
    QLabel* urllabel;
    QAction* exitFullScreen=new QAction();
    QAction* timeAction=new QAction();
    FullScreenNotifier* notifier=new FullScreenNotifier();
    TimeNotifier* timeNotifier=new TimeNotifier();
    QWebEngineView* view=new QWebEngineView();
    QLabel* correction=new QLabel();
    CrustaSpeech* c_speech=new CrustaSpeech();
    void createWebView();
    WebView *returnView();
    void home();
    void spinnerStarted();
    void faviconChanged(QIcon fav);
    void pageTitleChanged();
    void acceptFullScreen(QWebEngineFullScreenRequest request);
    void ExitAction();
    void download(QWebEngineDownloadItem* download_item);
    void downloadFinished(QString);
    void permissionHandler(const QUrl &securityOrigin, QWebEnginePage::Feature feature);
    void showLinkHovered(QString url);
    void closeTab();
    void audioInfo();
    void showContextMenu(const QPoint &pos);
    void loadFinished();
    void handleBeforePdf(qint64,qint64);
    void espeak();
    void search(QString text);
    void downloadLink();
    void downloadImage();
    void loadPermissions();
    WebView();
};



#endif // WEBVIEW_H
