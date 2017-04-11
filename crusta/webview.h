#ifndef WEBVIEW_H
#define WEBVIEW_H

#include "fullscreennotifier.h"
#include "timenotifier.h"

#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineSettings>
#include <QWebEngineFullScreenRequest>
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



class WebView:public QWebEngineView{
    Q_OBJECT
protected:
    QWebEngineView* createWindow(QWebEnginePage::WebWindowType type);
public:
    QString home_page=QString("https://google.com");
    QLayout* layout;
    QAction* exitFullScreen=new QAction();
    QAction* timeAction=new QAction();
    FullScreenNotifier* notifier=new FullScreenNotifier();
    TimeNotifier* timeNotifier=new TimeNotifier();
    QWebEngineView* view=new QWebEngineView();
    void createWebView();
    QWebEngineView* returnView();
    void home();
    void spinnerStarted();
    void faviconChanged(QIcon fav);
    void pageTitleChanged();
    void pageLoaded();
    void acceptFullScreen(QWebEngineFullScreenRequest request);
    void ExitAction();
    WebView();
};



#endif // WEBVIEW_H
