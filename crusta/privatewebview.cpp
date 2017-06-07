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
#include "fullscreennotifier.h"
#include "timenotifier.h"
#include "popup.h"
#include "privatemainview.h"
#include "downloadmanager.h"

#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineFullScreenRequest>
#include <QString>
#include <QStringList>
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
#include <QDir>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QRadioButton>
#include <QFileIconProvider>
#include <QGroupBox>
#include <QDesktopServices>
#include <QDialog>
#include <QFileDialog>

#include <iostream>




PrivateWebView::PrivateWebView(){
    exitFullScreen->setShortcut(Qt::Key_Escape);
    createPrivateWebView();
    settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,true);
    settings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);
    settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows,true);
    connect(this->page(),&QWebEnginePage::fullScreenRequested,this,&PrivateWebView::acceptFullScreen);
    connect(this->page(),&QWebEnginePage::loadStarted,this,&PrivateWebView::spinnerStarted);
    connect(this->page(),&QWebEnginePage::loadFinished,this,&PrivateWebView::loadFinished);
    connect(this->page(),&QWebEnginePage::iconChanged,this,&PrivateWebView::faviconChanged);
    connect(this->page(),&QWebEnginePage::titleChanged,this,&PrivateWebView::pageTitleChanged);
    connect(this->page(),&QWebEnginePage::featurePermissionRequested,this,&PrivateWebView::permissionHandler);
    connect(exitFullScreen,&QAction::triggered,this,&PrivateWebView::ExitAction);
    connect(page()->profile(),&QWebEngineProfile::downloadRequested,this,&PrivateWebView::download);
    connect(page(),&QWebEnginePage::linkHovered,this,&PrivateWebView::showLinkHovered);
    connect(page(),&QWebEnginePage::windowCloseRequested,this,&PrivateWebView::closeTab);
    //connect(page(),&QWebEnginePage::recentlyAudibleChanged,this,&PrivateWebView::audioInfo);
    connect(page(),&QWebEnginePage::authenticationRequired,this,&PrivateWebView::authenticate);
    connect(page(),&WebPage::windowCloseRequested,this,&PrivateWebView::closeTab);

    //context-menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&PrivateWebView::customContextMenuRequested,this,&PrivateWebView::showContextMenu);
}

void PrivateWebView::createPrivateWebView(){
    setPage(privatewebpage);
}

PrivateWebView* PrivateWebView::returnPrivateView(){
    return this;
}

void PrivateWebView::home(){
    load(QUrl(this->home_page));
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
            PrivateWebView* v=(PrivateWebView*)l->itemAt(1)->widget();
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
                if(!page()->recentlyAudible())
                    tabwidget->tabBar()->setTabButton(i,QTabBar::LeftSide,lab);
            }
        }
    }
    catch(...){
        return;
    }
}

void PrivateWebView::pageTitleChanged(){
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
        request.accept();
        wasFullScreened=true;
        widget=(QWidget*)this->parent();
        layout=widget->layout();
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
        notifier->setParent(0);
        timeNotifier->setParent(0);
        setParent(widget);
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
        view->raise();
        PrivateTabWindow* tabwin=new PrivateTabWindow();
        tabwin->vbox->setContentsMargins(0,0,0,0);
        tabwin->setWebView(view);
        tabwin->createControls();
        QWidget* widget=(QWidget*)this->parent();
        QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
        QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
        PWindow* win=(PWindow*)tabwidget->parentWidget();
        tabwin->menu_btn->setMenu(win->menu);
        tabwidget->insertTab(tabwidget->currentIndex()+1,tabwin->tab,tr("Connecting..."));
        tabwidget->setCurrentIndex(tabwidget->currentIndex()+1);
        return view;
    }
    case QWebEnginePage::WebBrowserBackgroundTab: {
        PrivateWebView* view=new PrivateWebView();
        view->raise();
        PrivateTabWindow* tabwin=new PrivateTabWindow();
        tabwin->vbox->setContentsMargins(0,0,0,0);
        tabwin->setWebView(view);
        tabwin->createControls();
        QWidget* widget=(QWidget*)this->parent();
        QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
        QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
        PWindow* win=(PWindow*)tabwidget->parentWidget();
        tabwin->menu_btn->setMenu(win->menu);
        tabwidget->insertTab(tabwidget->currentIndex()+1,tabwin->tab,tr("Connecting"));
        return view;
    }
    case QWebEnginePage::WebBrowserWindow: {
        std::cout<<"window Requested"<<std::endl;
        QWebEngineView* view=new QWebEngineView();
        connect(view->page(),&QWebEnginePage::windowCloseRequested,view,&QWebEngineView::close);
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
    QDialog* dg=new QDialog();
    QVBoxLayout* vl=new QVBoxLayout();
    dg->setLayout(vl);
    dg->setModal(true);
    dg->setWindowFlags(Qt::FramelessWindowHint);
    dg->setFixedWidth(400);
    QHBoxLayout* h0=new QHBoxLayout();
    QLabel* permission=new QLabel();
    h0->addWidget(permission);
    vl->addLayout(h0);
    QHBoxLayout* h1=new QHBoxLayout();
    h1->addWidget(new QLabel());
    QPushButton* cncl=new QPushButton();
    QPushButton* ok=new QPushButton();
    h1->addWidget(cncl);
    h1->addWidget(ok);
    vl->addLayout(h1);
    cncl->setText(tr("No"));
    ok->setText(tr("Yes"));
    dg->setStyleSheet("QWidget{color:black;background-color:white} QPushButton{border:0.5px solid;background-color:black;color:white;border-radius:0px;border-color:black;padding:2px 4px;} QPushButton:hover{background-color:white;color:black}");
    connect(cncl,&QPushButton::clicked,dg,&QDialog::reject);
    connect(ok,&QPushButton::clicked,dg,&QDialog::accept);
    switch (feature) {
    case QWebEnginePage::Notifications:{
        permission->setText(tr("Allow Notifications from this website"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    case QWebEnginePage::MouseLock:{
        permission->setText(tr("Allow Pointer Lock for this website"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    case QWebEnginePage::Geolocation:{
        permission->setText(tr("Allow Geolocation from current website"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    case QWebEnginePage::MediaAudioCapture:{
        permission->setText(tr("Allow Audio Capture for this site"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    case QWebEnginePage::MediaVideoCapture:{
        permission->setText(tr("Allow Video Capture for this site"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    case QWebEnginePage::MediaAudioVideoCapture:{
        permission->setText(tr("Allow Audio/Video Capture for this site"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    }
}

void PrivateWebView::download(QWebEngineDownloadItem *download_item){
    if(download_item->url().toString()!=link && download_item->url().toString().endsWith(".pdf")){
        return;
    }

    if(download_item->state()!=QWebEngineDownloadItem::DownloadRequested){
        return;
    }

    if(download_item->path().endsWith(".pdf")){
        QString path=download_item->path();
        QString cpath=QDir::tempPath();
        cpath+="/"+path.split('/')[path.split('/').length()-1];
        download_item->setPath(cpath);
        download_item->accept();
        QDir* viewer_file=new QDir(QCoreApplication::applicationDirPath());
        viewer_file->cd("../3rd_party/pdfjs/web");
        connect(download_item,&QWebEngineDownloadItem::downloadProgress,this,&PrivateWebView::handleBeforePdf);
        connect(download_item,&QWebEngineDownloadItem::finished,this,[this,cpath,viewer_file]{this->load(QUrl("file://"+viewer_file->absolutePath()+"/viewer.html?file="+cpath));});
        return;
    }

    QString path=download_item->path();
    QDialog* w=new QDialog();
    w->setWindowFlags(Qt::FramelessWindowHint);
    QVBoxLayout* box=new QVBoxLayout();
    w->setLayout(box);
    QLabel* yhcto=new QLabel(tr("You have choosen to open"));
    yhcto->setFont(QFont("Ariel",-1,-1,true));
    box->addWidget(yhcto);
    QHBoxLayout* hbx=new QHBoxLayout();
    QFileIconProvider* fip=new QFileIconProvider();
    QIcon icon=fip->icon(QFileInfo(path));
    QLabel* ilabel=new QLabel();
    ilabel->setPixmap(icon.pixmap(64,64));
    hbx->addWidget(ilabel);
    hbx->addWidget(new QLabel(path.split('/')[path.split('/').length()-1]+"\n"+tr("which is : ")+download_item->mimeType()));
    box->addLayout(hbx);
    QRadioButton* ropen=new QRadioButton();
    QRadioButton* rsave=new QRadioButton();
    ropen->setText(tr("Open..."));
    rsave->setText(tr("Save"));
    QGroupBox* gb=new QGroupBox();
    QVBoxLayout* vgb=new QVBoxLayout();
    vgb->addWidget(ropen);
    vgb->addWidget(rsave);
    rsave->setChecked(true);
    gb->setLayout(vgb);
    gb->setFlat(true);
    gb->setTitle(tr("What should Crusta do with this file?"));
    box->addWidget(gb);
    QPushButton* cncl_btn=new QPushButton();
    cncl_btn->setFixedWidth(75);
    cncl_btn->setText(tr("Cancel"));
    QPushButton* ok_btn=new QPushButton();
    ok_btn->setFixedWidth(75);
    ok_btn->setText(tr("Ok"));
    QHBoxLayout* h1bx=new QHBoxLayout();
    h1bx->addWidget(new QLabel());
    h1bx->addWidget(cncl_btn);
    h1bx->addWidget(ok_btn);
    box->addLayout(h1bx);
    w->setStyleSheet("QDialog{background-color:white;color:black} QLabel{color:black} QGroupBox{color:black;} QRadioButton{color:black;} QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
    connect(cncl_btn,&QPushButton::clicked,w,&QDialog::reject);
    connect(ok_btn,&QPushButton::clicked,w,&QDialog::accept);
    if(w->exec()!=QDialog::Accepted){
        download_item->cancel();
        return;
    }

    DownloadWidget* dw=new DownloadWidget();
    dw->getIcon(icon);
    dw->setFixedHeight(dw->sizeHint().height());
    connect(download_item,&QWebEngineDownloadItem::downloadProgress,dw,&DownloadWidget::computeFraction);
    connect(dw->cancel,&QPushButton::clicked,download_item,&QWebEngineDownloadItem::cancel);
    if(ropen->isChecked()){
        QString cpath=QDir::tempPath();
        cpath+="/"+path.split('/')[path.split('/').length()-1];
        download_item->setPath(cpath);
        download_item->accept();
        connect(download_item,&QWebEngineDownloadItem::finished,this,[this,cpath]{downloadFinished(cpath);});
    }
    else{
        QFileDialog f;
        f.setOption(QFileDialog::DontUseNativeDialog,true);
        QString fname=f.getSaveFileName(this,tr("Download File"),QDir::homePath(),QString(),nullptr,f.options());
        if(fname=="")fname=path;
        download_item->setPath(fname);
        download_item->accept();
    }
    connect(download_item,&QWebEngineDownloadItem::stateChanged,dw,&DownloadWidget::stateChanged);
    QString name=download_item->path().split('/')[download_item->path().split('/').length()-1];
    dw->getName(name);
    dw->path=download_item->path();
    QWidget* widget=(QWidget*)this->parent();
    QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
    QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
    PWindow* win=(PWindow*)tabwidget->parentWidget();
    win->d_manager->addDownloadItem(dw);
    QDir* exec_dir=new QDir(QCoreApplication::applicationDirPath());
    exec_dir->cd("../crusta_db");
    QFile file(exec_dir->absolutePath()+"/downloads.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream in(&file);
    in << download_item->path()+"\n";
    file.close();
}

void PrivateWebView::handleBeforePdf(qint64 recieved,qint64 total){
    if(total==0)
        return;
    returnPrivateView()->page()->runJavaScript("document.body.innerHTML = ''");
    returnPrivateView()->page()->runJavaScript("document.write('Loading ...');");
    returnPrivateView()->page()->runJavaScript("document.write("+QString::number((recieved*100)/total)+");");
    returnPrivateView()->page()->runJavaScript("document.write('%')");
}

void PrivateWebView::downloadFinished(QString path){
    QUrl u;
    u=QUrl::fromLocalFile(path);
    QDesktopServices::openUrl(u);
}

void PrivateWebView::showLinkHovered(QString url){
    link=url;
    //TODO : make a link hovered showing QLabel;
}

void PrivateWebView::closeTab(){
    QWidget* widget=(QWidget*)this->parent();
    QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
    QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
    for(int i=0;i<tabwidget->count();i++){
        QWidget* w=tabwidget->widget(i);
        QLayout* l=w->layout();
        QWebEngineView* v=(QWebEngineView*)l->itemAt(1)->widget();
        if(v==this){
            v->load(QUrl("http://"));
            v->disconnect();
            v->deleteLater();
            tabwidget->removeTab(i);
            break;
        }
    }
}

void PrivateWebView::audioInfo(){
    if(this->parent()==NULL)return;
    QWidget* widget=(QWidget*)this->parent();
    QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
    QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
    for(int i=0;i<tabwidget->count();i++){
        QWidget* w=tabwidget->widget(i);
        QLayout* l=w->layout();
        QWebEngineView* v=(QWebEngineView*)l->itemAt(1)->widget();
        if(v==this){
            QIcon ipix=QIcon(":/res/drawables/volume_max.png");
            QPixmap pix=ipix.pixmap(16,16);
            QLabel* lab=new QLabel();
            lab->setPixmap(pix);
            tabwidget->tabBar()->setTabButton(i,QTabBar::LeftSide,lab);
            break;
        }
    }
}

void PrivateWebView::authenticate(const QUrl u,QAuthenticator *authenticator){
    std::cout<<"yes"<<std::endl;
}

void PrivateWebView::showContextMenu(const QPoint& pos){
    QMenu* contextMenu=new QMenu();
    if(link!=""){
        QAction* open_link_in_new_tab=new QAction(tr("Open Link In New Private Tab"));
        connect(open_link_in_new_tab,&QAction::triggered,this,[this]{triggerPageAction(QWebEnginePage::OpenLinkInNewTab);});
        contextMenu->addAction(open_link_in_new_tab);
        QAction* open_link_in_new_window=new QAction(tr("Open Link as PopUp"));
        connect(open_link_in_new_window,&QAction::triggered,this,[this]{triggerPageAction(QWebEnginePage::OpenLinkInNewWindow);});
        contextMenu->addAction(open_link_in_new_window);
        contextMenu->addSeparator();
        QAction* follow_link=new QAction(tr("Follow Link"));
        connect(follow_link,&QAction::triggered,this,[this]{triggerPageAction(QWebEnginePage::OpenLinkInThisWindow);});
        contextMenu->addAction(follow_link);
        QAction* copy_link=new QAction(tr("Copy Link Address"));
        connect(copy_link,&QAction::triggered,this,[this]{triggerPageAction(QWebEnginePage::CopyLinkToClipboard);});
        contextMenu->addAction(copy_link);
        contextMenu->addSeparator();
    }
    else if(this->selectedText()!=""){
        QString text=this->selectedText();
        QAction* a_cut=new QAction(tr("Cut"));
        connect(a_cut,&QAction::triggered,this,[this]{triggerPageAction(QWebEnginePage::Cut);});
        contextMenu->addAction(a_cut);
        QAction* a_copy=new QAction(tr("Copy"));
        connect(a_copy,&QAction::triggered,this,[this]{triggerPageAction(QWebEnginePage::Copy);});
        contextMenu->addAction(a_copy);
        QAction* a_paste=new QAction(tr("Paste"));
        connect(a_paste,&QAction::triggered,this,[this]{triggerPageAction(QWebEnginePage::Paste);});
        contextMenu->addAction(a_paste);
        contextMenu->addSeparator();
        QAction* crusta_speak=new QAction(tr("Crusta Speak"));
        connect(crusta_speak,&QAction::triggered,this,&PrivateWebView::espeak);
        contextMenu->addAction(crusta_speak);
        contextMenu->addSeparator();
    }
    QAction* back_page=new QAction(QIcon(":/res/drawables/back.svg"),tr("Back"));
    connect(back_page,&QAction::triggered,this,&PrivateWebView::back);
    contextMenu->addAction(back_page);
    QAction* forward_page=new QAction(QIcon(":/res/drawables/forward.svg"),tr("Forward"));
    connect(forward_page,&QAction::triggered,this,&PrivateWebView::forward);
    contextMenu->addAction(forward_page);
    QAction* reload_page=new QAction(QIcon(":/res/drawables/reload.svg"),tr("Reload"));
    connect(reload_page,&QAction::triggered,this,&PrivateWebView::reload);
    contextMenu->addAction(reload_page);
    QAction* reload_and_bypass_cache=new QAction(tr("Reload And Bypass Cache"));
    connect(reload_and_bypass_cache,&QAction::triggered,this,[this]{triggerPageAction(QWebEnginePage::ReloadAndBypassCache);});
    contextMenu->addAction(reload_and_bypass_cache);
    contextMenu->addSeparator();
    QAction* view_page_source=new QAction(tr("View Page Source"));
    connect(view_page_source,&QAction::triggered,this,[this]{triggerPageAction(QWebEnginePage::ViewSource);});
    contextMenu->addAction(view_page_source);
    contextMenu->exec(this->mapToGlobal(pos));
}

void PrivateWebView::espeak(){
    QString txt=this->selectedText();
    system("espeak \""+txt.toLatin1()+"\"&");
}











