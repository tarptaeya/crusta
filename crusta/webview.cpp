/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017 Anmol Gautam <anmol@crustabrowser.com>
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
#include "tabwindow.h"
#include "fullscreennotifier.h"
#include "timenotifier.h"
#include "popup.h"
#include "mainview.h"
#include "downloadmanager.h"
#include "statusbar.h"

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
#include <QProcess>

#include <iostream>




WebView::WebView(){
    exitFullScreen->setShortcut(Qt::Key_Escape);
    createWebView();
    //urllabel = new QLabel(this);
    settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,true);
    settings()->setAttribute(QWebEngineSettings::PluginsEnabled,true);
    settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows,true);
    settings()->setAttribute(QWebEngineSettings::WebGLEnabled,true);
    settings()->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled,true);
    connect(this->page(),&QWebEnginePage::fullScreenRequested,this,&WebView::acceptFullScreen);
    connect(this->page(),&QWebEnginePage::loadStarted,this,&WebView::spinnerStarted);
    connect(this->page(),&QWebEnginePage::loadFinished,this,&WebView::loadFinished);
    connect(this->page(),&QWebEnginePage::iconChanged,this,&WebView::faviconChanged);
    connect(this->page(),&QWebEnginePage::titleChanged,this,&WebView::pageTitleChanged);
    connect(this->page(),&QWebEnginePage::featurePermissionRequested,this,&WebView::permissionHandler);
    connect(exitFullScreen,&QAction::triggered,this,&WebView::ExitAction);
    connect(page()->profile(),&QWebEngineProfile::downloadRequested,this,&WebView::download);
    connect(page(),&QWebEnginePage::linkHovered,this,&WebView::showLinkHovered);
    connect(page(),&QWebEnginePage::windowCloseRequested,this,&WebView::closeTab);
    //connect(page(),&QWebEnginePage::recentlyAudibleChanged,this,&WebView::audioInfo);
    connect(page(),&WebPage::windowCloseRequested,this,&WebView::closeTab);

    //context-menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&WebView::customContextMenuRequested,this,&WebView::showContextMenu);
}

void WebView::createWebView(){
    setPage(webpage);
}

WebView* WebView::returnView(){
    return this;
}

void WebView::home(){
    load(QUrl(this->home_page));
}

void WebView::spinnerStarted(){
    if(parent()==NULL)return;
    try{
        QWidget* widget=(QWidget*)this->parent();
        QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
        QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
        int count=tabwidget->count();
        for(int i=0;i<count;i++){
            QWidget* w=tabwidget->widget(i);
            QLayout* l=w->layout();
            WebView* v=(WebView*)l->itemAt(1)->widget();
            if(v==this){
                QLabel* spinner=new QLabel();
                QMovie* loader;
                if(tabwidget->currentIndex()==i)
                    loader=new QMovie(":/res/videos/loader.gif");
                else
                    loader=new QMovie(":/res/videos/passive_loader.gif");
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

void WebView::faviconChanged(QIcon fav){
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

void WebView::pageTitleChanged(){
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
                tabwidget->setTabToolTip(i,title);
            }
        }
    }
    catch(...){
        return;
    }
}

void WebView::acceptFullScreen(QWebEngineFullScreenRequest request){
    if(request.toggleOn()){
        request.accept();
        widget=(QWidget*)this->parent();
        layout=widget->layout();
        layout->replaceWidget(this,correction);
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
        notifier->hideNotifier();
        timeNotifier->setParent(0);
        timeNotifier->hideNotifier();
        setParent(widget);
        layout->replaceWidget(correction,this);
        removeAction(exitFullScreen);
        removeAction(timeAction);
    }
}

void WebView::ExitAction(){
    page()->triggerAction(QWebEnginePage::ExitFullScreen);
}

QWebEngineView* WebView::createWindow(QWebEnginePage::WebWindowType type){
    switch(type){
    case QWebEnginePage::WebBrowserTab: {
        WebView* view=new WebView();
        view->raise();
        TabWindow* tabwin=new TabWindow();
        tabwin->vbox->setContentsMargins(0,0,0,0);
        tabwin->setWebView(view);
        tabwin->createControls();
        QWidget* widget=(QWidget*)this->parent();
        QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
        QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
        Window* win=(Window*)tabwidget->parentWidget();
        tabwin->menu_btn->setMenu(win->menu);
        tabwidget->insertTab(tabwidget->currentIndex()+1,tabwin->tab,tr("Connecting..."));
        tabwidget->setCurrentIndex(tabwidget->currentIndex()+1);
        tabwidget->setCurrentIndex(tabwidget->currentIndex()-1);
        return view;
    }
    case QWebEnginePage::WebBrowserBackgroundTab: {
        WebView* view=new WebView();
        view->raise();
        TabWindow* tabwin=new TabWindow();
        tabwin->vbox->setContentsMargins(0,0,0,0);
        tabwin->setWebView(view);
        tabwin->createControls();
        QWidget* widget=(QWidget*)this->parent();
        QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
        QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
        Window* win=(Window*)tabwidget->parentWidget();
        tabwin->menu_btn->setMenu(win->menu);
        tabwidget->insertTab(tabwidget->currentIndex()+1,tabwin->tab,tr("Connecting..."));
        return view;
    }
    case QWebEnginePage::WebBrowserWindow: {
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

void WebView::permissionHandler(const QUrl &securityOrigin, QWebEnginePage::Feature feature){
    loadPermissions();
    QDialog* dg=new QDialog();
    QVBoxLayout* vl=new QVBoxLayout();
    dg->setLayout(vl);
    dg->setModal(true);
    dg->setWindowTitle("Crusta : Premission Requested by WebPage");
    dg->setFixedWidth(400);
    QHBoxLayout* h0=new QHBoxLayout();
    QLabel* permission=new QLabel();
    h0->addWidget(permission);
    vl->addLayout(h0);
    QHBoxLayout* h1=new QHBoxLayout();
    h1->addWidget(new QLabel());
    QPushButton* cncl=new QPushButton();
    QPushButton* ok=new QPushButton();
    ok->setDefault(true);
    h1->addWidget(cncl);
    h1->addWidget(ok);
    vl->addLayout(h1);
    cncl->setText(tr("No"));
    ok->setText(tr("Yes"));
    //dg->setStyleSheet("QWidget{color:black;background-color:white} QPushButton{border:0.5px solid;background-color:black;color:white;border-radius:0px;border-color:black;padding:2px 4px;} QPushButton:hover{background-color:white;color:black}");
    connect(cncl,&QPushButton::clicked,dg,&QDialog::reject);
    connect(ok,&QPushButton::clicked,dg,&QDialog::accept);
    switch (feature) {
    case QWebEnginePage::Notifications:{
        if(notif==0){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
            return;
        }
        else if(notif==2){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }

        permission->setText(tr("Allow Notifications from this website"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    case QWebEnginePage::MouseLock:{
        if(mouse==0){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
            return;
        }
        else if(mouse==2){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }

        permission->setText(tr("Allow Pointer Lock for this website"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    case QWebEnginePage::Geolocation:{
        if(geo==0){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
            return;
        }
        else if(geo==2){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }

        permission->setText(tr("Allow Geolocation from current website"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    case QWebEnginePage::MediaAudioCapture:{
        if(audio==0){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
            return;
        }
        else if(audio==2){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }

        permission->setText(tr("Allow Audio Capture for this site"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    case QWebEnginePage::MediaVideoCapture:{
        if(video==0){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
            return;
        }
        else if(video==2){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }

        permission->setText(tr("Allow Video Capture for this site"));
        if(dg->exec()!=QDialog::Accepted){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }
        page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
        break;
    }
    case QWebEnginePage::MediaAudioVideoCapture:{
        if(av==0){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionGrantedByUser);
            return;
        }
        else if(av==2){
            page()->setFeaturePermission(securityOrigin,feature,QWebEnginePage::PermissionDeniedByUser);
            return;
        }

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

void WebView::download(QWebEngineDownloadItem *download_item){
    if(download_item->state()!=QWebEngineDownloadItem::DownloadRequested){
        return;
    }
    QString path=download_item->path();
    QDialog* w=new QDialog();
    w->setWindowTitle("Crusta : Download Requested");
    QVBoxLayout* box=new QVBoxLayout();
    w->setLayout(box);
    QLabel* yhcto=new QLabel(tr("You have choosen to open"));
    yhcto->setFont(QFont("Ariel",-1,-1,true));
    box->addWidget(yhcto);
    QHBoxLayout* hbx=new QHBoxLayout();
    QIcon icon;
    if(download_item->mimeType().contains("audio"))
        icon=QIcon(":/res/drawables/music_download.png");
    else if(download_item->mimeType().contains("pdf"))
        icon=QIcon(":/res/drawables/pdf_download.png");
    else if(download_item->mimeType().contains("image"))
        icon=QIcon(":/res/drawables/image_download.png");
    else if(download_item->mimeType().contains("zip"))
        icon=QIcon(":/res/drawables/zip_download.png");
    else if(download_item->mimeType().contains("x-msdownload"))
        icon=QIcon(":/res/drawables/exe_download.png");
    else if(download_item->mimeType().contains("video"))
        icon=QIcon(":/res/drawables/video_download.png");
    else
        icon=QIcon(":/res/drawables/file_download.png");
    w->setWindowIcon(icon);
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
    QRadioButton* specifirDownloadOption = new QRadioButton(); // BITS on windows, wget on ubuntu
    QString platform = QSysInfo().productType();
    if(platform == "windows"){
        specifirDownloadOption->setText(tr("use BITS (recommonded for large files)"));
        vgb->addWidget(specifirDownloadOption);
        specifirDownloadOption->setChecked(true);
    } else if(platform == "ubuntu"){
        specifirDownloadOption->setStyleSheet("font-weight: bold");
        specifirDownloadOption->setText(tr("use wget"));
        vgb->addWidget(specifirDownloadOption);
        specifirDownloadOption->setChecked(true);
    } else {
        rsave->setChecked(true);
    }
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
    ok_btn->setDefault(true);
    QHBoxLayout* h1bx=new QHBoxLayout();
    h1bx->addWidget(new QLabel());
    h1bx->addWidget(cncl_btn);
    h1bx->addWidget(ok_btn);
    box->addLayout(h1bx);
    //w->setStyleSheet("QDialog{background-color:white;color:black} QLabel{color:black} QGroupBox{color:black;} QRadioButton{color:black;} QPushButton{border:0.5px solid black;padding:4px 8px;color:white;background-color:black} QPushButton:hover{background-color:white;color:black}");
    connect(cncl_btn,&QPushButton::clicked,w,&QDialog::reject);
    connect(ok_btn,&QPushButton::clicked,w,&QDialog::accept);
    if(w->exec()!=QDialog::Accepted){
        download_item->cancel();
        return;
    }
    if(platform == "windows"){
        if(specifirDownloadOption->isChecked()){
            QProcess::startDetached("powershell -Command \"Import-Module BitsTransfer; Start-BitsTransfer "+download_item->url().toString()+" "+download_item->path());
            download_item->cancel();
            return;
        }
    } else if(platform == "ubuntu"){
        if(specifirDownloadOption->isChecked()){
            QStringList directory_prefix_list = download_item->path().split('/');
            directory_prefix_list.removeLast();
            QString directory_prefix = directory_prefix_list.join("/");
            QProcess::startDetached("wget --directory-prefix="+directory_prefix+" --tries=5 "+download_item->url().toString());
            download_item->cancel();
            return;
        }
    }

    DownloadWidget* dw=new DownloadWidget();
    dw->getIcon(icon);
    dw->setFixedHeight(dw->sizeHint().height());
    connect(download_item,&QWebEngineDownloadItem::downloadProgress,dw,&DownloadWidget::computeFraction);
    connect(dw->cancel,&QPushButton::clicked,download_item,&QWebEngineDownloadItem::cancel);
    if(ropen->isChecked()){
        if(download_item->path().endsWith(".pdf")){
            QString path=download_item->path();
            QString cpath=QDir::tempPath();
            cpath+="/"+path.split('/')[path.split('/').length()-1];
            download_item->setPath(cpath);


            WebView* view=new WebView();
            TabWindow* tabwin=new TabWindow();
            tabwin->vbox->setContentsMargins(0,0,0,0);
            tabwin->setWebView(view);
            tabwin->createControls();
            QWidget* widget=(QWidget*)this->parent();
            QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
            QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
            Window* win=(Window*)tabwidget->parentWidget();
            tabwin->menu_btn->setMenu(win->menu);
            tabwidget->insertTab(tabwidget->currentIndex()+1,tabwin->tab,tr("Connecting..."));
            tabwidget->setCurrentIndex(tabwidget->currentIndex()+1);


            download_item->accept();
            QDir* viewer_file=new QDir(QCoreApplication::applicationDirPath());
            viewer_file->cd("3rd_party/pdfjs/web");
            connect(download_item,&QWebEngineDownloadItem::downloadProgress,view,&WebView::handleBeforePdf);
            if(viewer_file->absolutePath().startsWith("/"))
                connect(download_item,&QWebEngineDownloadItem::finished,view,[view,cpath,viewer_file]{view->load(QUrl("file://"+viewer_file->absolutePath()+"/viewer.html?file="+cpath));});
            else
                connect(download_item,&QWebEngineDownloadItem::finished,view,[view,cpath,viewer_file]{view->load(QUrl("file:///"+viewer_file->absolutePath()+"/viewer.html?file="+cpath));});
            return;
        }
        QString cpath=QDir::tempPath();
        cpath+="/"+path.split('/')[path.split('/').length()-1];
        download_item->setPath(cpath);
        download_item->accept();
        connect(download_item,&QWebEngineDownloadItem::finished,this,[this,cpath]{downloadFinished(cpath);});
    }
    else{
        QFileDialog f;
        QString fname=f.getSaveFileName(this,tr("Download File"),path,QString(),nullptr,f.options());
        if(fname=="")return;
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
    Window* win=(Window*)tabwidget->parentWidget();
    win->d_manager->addDownloadItem(dw);
    QFile file(QDir::homePath()+"/.crusta_db/downloads.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream in(&file);
    in << download_item->path()+"\n";
    file.close();
}

void WebView::handleBeforePdf(qint64 recieved,qint64 total){
    if(total==0)
        return;
    returnView()->page()->runJavaScript("document.body.innerHTML = ''");
    returnView()->page()->runJavaScript("document.write('Loading ...');");
    returnView()->page()->runJavaScript("document.write("+QString::number((recieved*100)/total)+");");
    returnView()->page()->runJavaScript("document.write('%')");
}

void WebView::downloadFinished(QString path){
    QUrl u;
    u=QUrl::fromLocalFile(path);
    QDesktopServices::openUrl(u);
}

void WebView::showLinkHovered(QString url){
    link=url;
}

void WebView::closeTab(){
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

void WebView::audioInfo(){
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

void WebView::downloadLink(){
    this->page()->triggerAction(QWebEnginePage::DownloadLinkToDisk);
}

void WebView::downloadImage(){
    this->page()->triggerAction(QWebEnginePage::DownloadImageToDisk);
}

void WebView::showContextMenu(const QPoint& pos){
    QMenu* contextMenu=new QMenu();
    if(link!="" && !link.endsWith(".pdf")){
        QAction* open_link_in_new_tab=new QAction(tr("Open Link In New Tab"));
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
        connect(crusta_speak,&QAction::triggered,this,&WebView::espeak);
        contextMenu->addAction(crusta_speak);
        QAction* a_search=new QAction(tr("Search"));
        connect(a_search,&QAction::triggered,this,[this,text]{this->search(text);});
        contextMenu->addAction(a_search);
        contextMenu->addSeparator();
    }
    if(link!=""){
        QMenu* download=new QMenu(tr("Download"));
        contextMenu->addMenu(download);
        QAction* download_img=new QAction(tr("Download Image"));
        connect(download_img,&QAction::triggered,this,&WebView::downloadImage);
        download->addAction(download_img);
        QAction* download_link=new QAction(tr("Download Link"));
        connect(download_link,&QAction::triggered,this,&WebView::downloadLink);
        download->addAction(download_link);
        contextMenu->addSeparator();
    }

    QAction* back_page=new QAction(QIcon(":/res/drawables/back.svg"),tr("Back"));
    connect(back_page,&QAction::triggered,this,&WebView::back);
    contextMenu->addAction(back_page);
    QAction* forward_page=new QAction(QIcon(":/res/drawables/forward.svg"),tr("Forward"));
    connect(forward_page,&QAction::triggered,this,&WebView::forward);
    contextMenu->addAction(forward_page);
    QAction* reload_page=new QAction(QIcon(":/res/drawables/reload.svg"),tr("Reload"));
    connect(reload_page,&QAction::triggered,this,&WebView::reload);
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

void WebView::loadFinished(){
    if(this->icon().isNull()){
        this->faviconChanged(QIcon());
    }
    QFile file(QDir::homePath()+"/.crusta_db/history.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out << title().toLatin1()+">>>>>"+url().toString().toLatin1()+">>>>>"+QDate::currentDate().toString().toLatin1()+"\n";
    file.close();
}

void WebView::espeak(){
    QString txt=this->selectedText();
    c_speech->tts->setText(txt);
    c_speech->show();
    c_speech->speak();
}

void WebView::search(QString text){
    QFile inputFile(QDir::homePath()+"/.crusta_db/settings.txt");
    QString srch;
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          QStringList data=line.split(">>>>>");
          if(data[0]=="engine"){
              srch=data[1];
              break;
          }
       }
       inputFile.close();
    }
    QWidget* widget=(QWidget*)this->parent();
    QStackedWidget* stackedwidget=(QStackedWidget*)widget->parent();
    QTabWidget* tabwidget=(QTabWidget*)stackedwidget->parent();
    Window* win=(Window*)tabwidget->parentWidget();
    win->parentView->openUrl(srch+text);
}

void WebView::loadPermissions(){
    QStringList slist;
    QFile f(QDir::homePath()+"/.crusta_db/permissions.txt");
    if(f.exists()){
        f.open(QIODevice::ReadOnly);
        QTextStream in(&f);
        while (!in.atEnd()){
           QString line = in.readLine();
           slist.append(line);
        }
        f.close();
    }
    if(slist[0]=="0")
        notif=0;
    else if(slist[0]=="1")
        notif=1;
    else
        notif=2;
    if(slist[1]=="0")
        mouse=0;
    else if(slist[1]=="1")
        mouse=1;
    else
        mouse=2;
    if(slist[2]=="0")
        geo=0;
    else if(slist[2]=="1")
        geo=1;
    else
        geo=2;
    if(slist[3]=="0")
        audio=0;
    else if(slist[3]=="1")
        audio=1;
    else
        audio=2;
    if(slist[4]=="0")
        video=0;
    else if(slist[4]=="1")
        video=1;
    else
        video=2;
    if(slist[5]=="0")
        av=0;
    else if(slist[5]=="1")
        av=1;
    else
        av=2;
}








