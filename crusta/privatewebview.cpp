/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017 Anmol Gautam <tarptaeya@gmail.com>
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

#include "privatewebview.h"
#include "privatetabwindow.h"
#include "popup.h"
#include "fullscreennotifier.h"
#include "featurenotifier.h"
#include "presentationmodenotifier.h"

#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QTabBar>
#include <QTabWidget>
#include <QStackedWidget>
#include <QMovie>
#include <QWidget>
#include <QLabel>
#include <QLayout>

#include <iostream>

PrivateWebView::PrivateWebView(){
    QWebEngineProfile* profile=new QWebEngineProfile();
    profile->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
    QWebEnginePage* page=new QWebEnginePage(profile);
    setPage(page);
    exitFullScreen->setShortcut(Qt::Key_Escape);
    createPrivateWebView();
    settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,true);
    settings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);
    settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows,true);
    connect(this->page(),&QWebEnginePage::fullScreenRequested,this,&PrivateWebView::acceptFullScreen);
    connect(this->page(),&QWebEnginePage::loadStarted,this,&PrivateWebView::spinnerStarted);
    connect(this->page(),&QWebEnginePage::featurePermissionRequested,this,&PrivateWebView::permissionHandler);
    connect(this->page(),&QWebEnginePage::iconChanged,this,&PrivateWebView::faviconChanged);
    connect(this->page(),&QWebEnginePage::titleChanged,this,&PrivateWebView::pageTitleChanged);

    connect(exitFullScreen,&QAction::triggered,this,&PrivateWebView::ExitAction);
}

void PrivateWebView::createPrivateWebView(){
    load(QUrl(home_page));
}

QWebEngineView* PrivateWebView::returnPrivateView(){
    return this;
}

void PrivateWebView::home(){
    load(QUrl(home_page));
}

void PrivateWebView::spinnerStarted(){
    if(parent()==NULL)return;
    try{
        QWidget* widget=(QWidget*)this->parent();
        QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
        QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
        int count=tabwidget->count();
        for(int i=0;i<count;i++){
            QWidget* w=tabwidget->widget(i);
            QLayout* l=w->layout();
            QWebEngineView* v=(QWebEngineView*)l->itemAt(1)->widget();
            if(v==this){
                QLabel* spinner=new QLabel();
                QMovie* loader=new QMovie(":/res/videos/loader.gif");
                loader->start();
                spinner->setMovie(loader);
                tabwidget->tabBar()->setTabButton(i,QTabBar::LeftSide,spinner);
            }
        }
    }
    catch(...){
        return;
    }
}

void PrivateWebView::faviconChanged(QIcon fav){
    if(parent()==NULL)return;
    try{
        QWidget* widget=(QWidget*)this->parent();
        QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
        QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
        int count=tabwidget->count();
        for(int i=0;i<count;i++){
            QWidget* w=tabwidget->widget(i);
            QLayout* l=w->layout();
            QWebEngineView* v=(QWebEngineView*)l->itemAt(1)->widget();
            if(v==this){
                QPixmap pix=fav.pixmap(16,16);
                QLabel* lab=new QLabel();
                lab->setPixmap(pix);
                tabwidget->tabBar()->setTabButton(i,QTabBar::LeftSide,lab);
            }
        }
    }
    catch(...){
        return;
    }
}

void PrivateWebView::pageTitleChanged(){
    page()->profile()->clearAllVisitedLinks();
    page()->profile()->clearHttpCache();
    if(parent()==NULL)return;
    try{
        QWidget* widget=(QWidget*)this->parent();
        QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
        QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
        int count=tabwidget->count();
        for(int i=0;i<count;i++){
            QWidget* w=tabwidget->widget(i);
            QLayout* l=w->layout();
            QWebEngineView* v=(QWebEngineView*)l->itemAt(1)->widget();
            if(v==this){
                QString title=this->page()->title();
                tabwidget->setTabText(i,title);
            }
        }
    }
    catch(...){
        return;
    }
}

void PrivateWebView::acceptFullScreen(QWebEngineFullScreenRequest request){
    if(request.toggleOn()){
        wasFullScreen=true;
        request.accept();
        QWidget* w=(QWidget*)this->parent();
        layout=w->layout();
        layout->removeWidget(this);
        addAction(exitFullScreen);
        setParent(0);
        showFullScreen();
        notifier->setViewParent(this);
        notifier->showNotifier();
        timeNotifier->setViewParent(this);
        timeAction->setShortcut(Qt::Key_T);
        connect(timeAction,&QAction::triggered,timeNotifier,&TimeNotifier::showNotifier);
        addAction(timeAction);
    }
    else{
        request.accept();
        notifier->hideNotifier();
        timeNotifier->hideNotifier();
        layout->addWidget(this);
        removeAction(exitFullScreen);
        removeAction(timeAction);
    }
}

void PrivateWebView::ExitAction(){
    page()->triggerAction(QWebEnginePage::ExitFullScreen);
}

QWebEngineView* PrivateWebView::createWindow(QWebEnginePage::WebWindowType type){
    switch(type){
    case QWebEnginePage::WebBrowserTab: {
        PrivateWebView* view=new PrivateWebView();
        view->raise(); // to prevent it from closing when the view which requests this gets closed
        PrivateTabWindow* tabwin=new PrivateTabWindow();
        tabwin->vbox->setContentsMargins(0,0,0,0);
        tabwin->setWebView(view);
        tabwin->createControls();
        QWidget* widget=(QWidget*)this->parent();
        QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
        QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
        tabwidget->insertTab(tabwidget->currentIndex()+1,tabwin->tab,"New Tab");
        tabwidget->setCurrentIndex(tabwidget->currentIndex()+1);
        return view;
    }
    case QWebEnginePage::WebBrowserBackgroundTab: {
        std::cout<<"Background Tab Requested"<<std::endl;
        return new QWebEngineView();
    }
    case QWebEnginePage::WebBrowserWindow: {
        std::cout<<"window Requested"<<std::endl;
        PrivateWebView* view=new PrivateWebView();
        connect(view->page(),&QWebEnginePage::windowCloseRequested,view,&PrivateWebView::close);
        view->setWindowTitle(view->title());
        view->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        view->show();
        view->move((this->width()-view->width())/2,(this->height()-view->height())/2);
        connect(view,SIGNAL(titleChanged(QString)),view,SLOT(setWindowTitle(QString)));
        return view;
    }
    case QWebEnginePage::WebDialog: {
        PopUp* popup=new PopUp();
        QWebEngineView* view=new QWebEngineView();
        popup->setWebView(view);
        popup->createView();
        popup->widget->show();
        connect(view->page(),&QWebEnginePage::windowCloseRequested,popup,&PopUp::close);
        popup->widget->move((this->width()-view->width())/2,(this->height()-view->height())/2);
        return view;
    }
    }
    return nullptr;
}

void PrivateWebView::permissionHandler(const QUrl &securityOrigin, QWebEnginePage::Feature feature){
    FeatureNotifier* featureNotifier=new FeatureNotifier();
    featureNotifier->setViewParent(this);
    switch (feature) {
    case QWebEnginePage::MouseLock:
        featureNotifier->createNotifier(QString("Mouse Lock is accepted - Press ESC to exit"));
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        featureNotifier->showNotifier();
        break;
    case QWebEnginePage::Geolocation:
        featureNotifier->createNotifier(QString("Geolocation request is accepted"));
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        featureNotifier->showNotifier();
        break;
    case QWebEnginePage::MediaAudioCapture:
        featureNotifier->createNotifier(QString("Media Audio Capture request is accepted"));
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        featureNotifier->showNotifier();
        break;
    case QWebEnginePage::MediaVideoCapture:
        featureNotifier->createNotifier(QString("Media Video Capture request is accepted"));
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        featureNotifier->showNotifier();
        break;
    case QWebEnginePage::MediaAudioVideoCapture:
        featureNotifier->createNotifier(QString("Media Audio-Video Capture request is accepted"));
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        featureNotifier->showNotifier();
        break;
    }
}









