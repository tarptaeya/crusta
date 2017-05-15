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

#include "mainview.h"

#include "fullscreennotifier.h"
#include "webview.h"
#include "searchlineedit.h"
#include "addresslineedit.h"
#include "tabwindow.h"
#include "privatetabwindow.h"
#include "presentationmodenotifier.h"
#include "jseditor.h"
#include "themeeditor.h"

#include <QObject>
#include <QPoint>
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QPushButton>
#include <QAction>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QDir>
#include <QStatusBar>
#include <QWebEngineDownloadItem>
#include <QPrinter>
#include <QPageSetupDialog>

#include <iostream>




void MainView::closeTab(int index){
    if(this->tabWindow->count()==1)QApplication::quit();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
//    if(!webview->wasFullScreened){
//        webview->deleteLater();
//    }
//    else{
//        webview->load(webview->home_page);
//    }
//    this->tabWindow->removeTab(index);
//    webview->disconnect();
//    if(!webview->page()->isAudioMuted()){
//        webview->page()->setAudioMuted(true);
//    }
    QAction* hist=new QAction();
    hist->setText(webview->title());
    hist->setIcon(webview->icon());
    QUrl u=webview->url();
    webview->page()->deleteLater();
    this->tabWindow->removeTab(index);
    MainView::addNewTabButton();
    this->recently_closed->addAction(hist);
    connect(hist,&QAction::triggered,this,[this,u]{restoreTab(u);});
}

void MainView::zoomIn(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->setZoomFactor(webview->zoomFactor()+.20);
}

void MainView::zoomOut(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->setZoomFactor(webview->zoomFactor()-.20);
}

void MainView::resetZoom(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->setZoomFactor(1);
}

void MainView::fullScreen(){
    if(this->window->isFullScreen()){
        this->window->showMaximized();
        this->fullscreen_action->setText("&Show Full Screen");
    }
    else{
        this->window->showFullScreen();
        this->fullscreen_action->setText("&Exit Full Screen");
    }
}


void MainView::tabBarContext(QPoint point){
    if(this->tabWindow->tabBar()->tabAt(point)!=-1){
        int index=this->tabWindow->tabBar()->tabAt(point);
        QWidget* widget=this->tabWindow->widget(index);
        QLayout* layout=widget->layout();
        QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();

        QMenu* contextMenu=new QMenu();
        QAction* rld_tab=new QAction();
        rld_tab=contextMenu->addAction("&Reload Tab");
        connect(rld_tab,&QAction::triggered,webview,&QWebEngineView::reload);
        contextMenu->addAction("&Mute Tab");
        contextMenu->addAction("&Pin Tab");
        QAction* duplicate=new QAction();
        duplicate=contextMenu->addAction("&Duplicate Tab");
        connect(duplicate,&QAction::triggered,this,[this,webview]{duplicateTab(webview);});
        contextMenu->addAction("&Open in new Window");
        contextMenu->addAction("&Bookmark Tab");
        contextMenu->addAction("&Bookmark All Tabs");
        QAction* closeoth=new QAction();
        closeoth=contextMenu->addAction("&Close Other Tabs");
        connect(closeoth,&QAction::triggered,this,[this,index]{closeOtherTabs(index);});
        contextMenu->setStyleSheet("QMenu{background-color:white;color:blueviolet} QMenu::selected{color:white;background-color:blueviolet}");
        contextMenu->exec(this->tabWindow->tabBar()->mapToGlobal(point));
    }
    else{
        QMenu* barContext=new QMenu();
        QAction* ntab_bar=new QAction();
        ntab_bar=barContext->addAction("&New Tab");
        connect(ntab_bar,&QAction::triggered,this,&MainView::addNormalTab);
        QAction* rlod=new QAction();
        rlod=barContext->addAction("&Reload All Tabs");
        connect(rlod,&QAction::triggered,this,&MainView::reloadAllTabs);
        barContext->addAction("&Bookmark All Tabs");
        barContext->addAction("Mute All Tabs");
        barContext->addAction("&Restore All Tabs");
        barContext->setStyleSheet("QMenu{background-color:white;color:blueviolet} QMenu::selected{color:white;background-color:blueviolet}");
        barContext->exec(this->tabWindow->tabBar()->mapToGlobal(point));
    }
}

void MainView::FindText(){
    try{
        int index=this->tabWindow->currentIndex();
        QWidget* widget=this->tabWindow->widget(index);
        QLayout* layout=widget->layout();
        QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
        this->hbox=new QHBoxLayout();
        this->findwidget->setParent(webview);
        this->findwidget->setLayout(this->hbox);
        this->hbox->addWidget(this->close_findwidget);
        this->hbox->addWidget(this->label);
        this->hbox->addWidget(this->text);
        this->hbox->addWidget(this->match_case_btn);
        this->hbox->addWidget(new QLabel());
        this->close_findwidget->setFlat(true);
        this->close_findwidget->setIcon(QIcon(":/res/drawables/close.png"));
        this->close_findwidget->setFixedWidth(30);
        connect(this->close_findwidget,&QPushButton::clicked,this,&MainView::hideFindWidget);
        this->label->setText(QString("Search"));
        this->label->setFixedWidth(75);
        this->text->setFixedWidth(380);
        connect(this->text,&QLineEdit::returnPressed,this,&MainView::findFindWidget);
        connect(this->match_case_btn,&QCheckBox::toggled,this,&MainView::findFindWidget);
        this->match_case_btn->setText("&Match &Case");
        this->hbox->setAlignment(Qt::AlignLeft);
        this->findwidget->setFixedHeight(50);
        this->findwidget->setFixedWidth(webview->geometry().width());
        this->findwidget->setObjectName("findwidget");
        this->findwidget->setStyleSheet("#findwidget{border-top:1px solid grey;background-color:#ffffff}");
        this->findwidget->show();
        QPropertyAnimation *animation = new QPropertyAnimation(this->findwidget, "pos");
        animation->setDuration(600);
        this->start_findwidget=webview->geometry().height();
        animation->setStartValue(QPoint(0,this->start_findwidget));
        animation->setEndValue(QPoint(0,this->start_findwidget-50));
        animation->start();
        this->text->setFocus();
    }
    catch(...){
        return;
    }
}

void MainView::findFindWidget(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    QString data=this->text->text();
    if(this->match_case_btn->isChecked()){
        webview->findText("");
        QWebEnginePage::FindFlags options={QWebEnginePage::FindCaseSensitively};
        webview->findText(data,options);
    }
    else{
        webview->findText("");
        webview->findText(data);
    }
}

void MainView::hideFindWidget(){
    QPropertyAnimation *animation = new QPropertyAnimation(this->findwidget, "pos");
    animation->setDuration(800);
    animation->setStartValue(QPoint(0,this->start_findwidget-50));
    animation->setEndValue(QPoint(0,this->start_findwidget));
    animation->start();
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->findText("");
    this->findwidget->setParent(0);
    this->text->setText("");
}

void MainView::selectAllText(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->triggerPageAction(QWebEnginePage::SelectAll);
}

void MainView::quit(){
    this->window->deleteLater();
}

void MainView::enterPresentationMode(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    QString qurl=webview->url().toString();
    QWebEngineView* newwebview=new QWebEngineView();
    newwebview->load(QUrl(qurl));
    newwebview->showFullScreen();
    QAction* newExitAction=new QAction();
    newExitAction->setShortcut(Qt::Key_Escape);
    newwebview->addAction(newExitAction);
    this->p_notifier->setViewParent(newwebview);
    this->p_notifier->showNotifier();
    connect(newExitAction,&QAction::triggered,newwebview,&QWebEngineView::close);
}

void MainView::undoPageAction(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->triggerPageAction(QWebEnginePage::Undo);
}

void MainView::redoPageAction(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->triggerPageAction(QWebEnginePage::Redo);
}

void MainView::cutPageAction(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->triggerPageAction(QWebEnginePage::Cut);
}

void MainView::copyPageAction(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->triggerPageAction(QWebEnginePage::Copy);
}

void MainView::pastePageAction(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->triggerPageAction(QWebEnginePage::Paste);
}

MainView::MainView(){
    this->box->setContentsMargins(0,0,0,0);
    this->tabWindow->tabBar()->setDocumentMode(true);
    this->tabWindow->setElideMode(Qt::ElideRight);
    //this->tabWindow->setStyleSheet("QTabBar::tab{max-width:175px;min-width:175px;} QTabWidget::tab-bar{left:0px;}");
    this->tabWindow->tabBar()->setExpanding(false);
    this->tabWindow->tabBar()->setShape(QTabBar::RoundedNorth);
    this->tabWindow->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->tabWindow->tabBar(),&QTabBar::tabCloseRequested,this,&MainView::closeTab);
    connect(this->tabWindow->tabBar(),&QTabBar::customContextMenuRequested,this,&MainView::tabBarContext);
    createView();
    createMenuBar();
    createTabWindow();
    addNormalTab();

    addNewTabButton();
    this->tabWindow->tabBar()->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    connect(this->tabWindow->tabBar(),&QTabBar::tabBarDoubleClicked,this,&MainView::tabAreaDoubleClicked);
    connect(this->tabWindow->tabBar(),&QTabBar::tabMoved,this,&MainView::addNewTabButton);
    connect(this->tabWindow,&QTabWidget::currentChanged,this,&MainView::addNewTabButton);
    connect(this->newtabbtn,&QPushButton::clicked,this,&MainView::addNormalTab);
    this->tabWindow->setStyleSheet("QTabWidget::tab-bar{left:0px;} QTabBar{background-color:blueviolet;} QTabBar::tab:selected{background-color:white;color:blueviolet;max-width:175px;min-width:175px;} QTabBar::tab:!selected{max-width:173px;min-width:173px;color:white;background-color:deepskyblue;top:2px;border:0.5px solid blueviolet} QPushButton{background-color:deepskyblue;} QPushButton:hover{background-color:white;}");
}

void MainView::createView(){
    this->window->setWindowTitle("Crusta");
    this->window->setLayout(box);
}

void MainView::showView(){
    this->window->showMaximized();
}

void MainView::newWindow(){
    MainView* newView=new MainView();
    newView->showView();
}

void MainView::createMenuBar(){
    this->file_menu=this->menubar->addMenu("&File");
    this->new_tab_action=this->file_menu->addAction("&New Tab");
    connect(this->new_tab_action,&QAction::triggered,this,&MainView::addNormalTab);
    this->file_menu->addAction("&New Split Tab");
    this->new_window_action=this->file_menu->addAction("&New Window");
    connect(this->new_window_action,&QAction::triggered,this,&MainView::newWindow);
    this->new_private_tab_action=this->file_menu->addAction("&New Private Tab");
    connect(new_private_tab_action,&QAction::triggered,this,&MainView::addPrivateTab);
    this->file_menu->addAction("&New Private Window");
    this->file_menu->addSeparator();
    this->open_file=this->file_menu->addAction("&Open File");
    connect(this->open_file,&QAction::triggered,this,&MainView::openLocalFile);
    this->save_as_pdf=this->file_menu->addAction("&Save Page As PDF");
    connect(this->save_as_pdf,&QAction::triggered,this,&MainView::saveAsPdf);
    this->save_page=this->file_menu->addAction("&Save Page");
    connect(this->save_page,&QAction::triggered,this,&MainView::savePage);
    this->capture_screenshot=this->file_menu->addAction("&Capture ScreenShot");
    this->capture_screenshot->setShortcut(QKeySequence(QKeySequence::Save));
    connect(this->capture_screenshot,&QAction::triggered,this,&MainView::screenShot);
    this->exit_action=this->file_menu->addAction("&Quit");
    connect(this->exit_action,&QAction::triggered,this,&MainView::quit);
    this->edit_menu=this->menubar->addMenu("&Edit");
    this->undo_action=this->edit_menu->addAction("&Undo");
    connect(this->undo_action,&QAction::triggered,this,&MainView::undoPageAction);
    this->redo_action=this->edit_menu->addAction("&Redo");
    connect(this->redo_action,&QAction::triggered,this,&MainView::redoPageAction);
    this->edit_menu->addSeparator();
    this->cut_action=this->edit_menu->addAction("&Cut");
    connect(this->cut_action,&QAction::triggered,this,&MainView::cutPageAction);
    this->copy_action=this->edit_menu->addAction("&Copy");
    connect(this->copy_action,&QAction::triggered,this,&MainView::copyPageAction);
    this->paste_action=this->edit_menu->addAction("&Paste");
    connect(this->paste_action,&QAction::triggered,this,&MainView::pastePageAction);
    this->edit_menu->addSeparator();
    this->selectall_action=this->edit_menu->addAction("&Select All");
    connect(this->selectall_action,&QAction::triggered,this,&MainView::selectAllText);
    this->find_action=this->edit_menu->addAction("&Find");
    this->find_action->setShortcut(QKeySequence(QKeySequence::Find));
    connect(this->find_action,&QAction::triggered,this,&MainView::FindText);
    this->preference=this->edit_menu->addAction("&Edit Preference");
    connect(this->preference,&QAction::triggered,this,&MainView::editPreference);
    this->view_menu=this->menubar->addMenu("&View");
    this->view_page_source_action=this->view_menu->addAction("&Page Source");
    connect(this->view_page_source_action,&QAction::triggered,this,&MainView::viewPageSource);
    this->zoom_in_action=this->view_menu->addAction("&Zoom In");
    connect(this->zoom_in_action,&QAction::triggered,this,&MainView::zoomIn);
    this->zoom_out_action=this->view_menu->addAction("&Zoom Out");
    connect(this->zoom_out_action,&QAction::triggered,this,&MainView::zoomOut);
    this->reset_zoom_action=this->view_menu->addAction("&Reset Zoom");
    connect(this->reset_zoom_action,&QAction::triggered,this,&MainView::resetZoom);
    this->view_menu->addSeparator();
    this->presentation_action=this->view_menu->addAction("&Presentation Mode");
    connect(this->presentation_action,&QAction::triggered,this,&MainView::enterPresentationMode);
    this->fullscreen_action=this->view_menu->addAction("&Show Full Screen");
    connect(this->fullscreen_action,&QAction::triggered,this,&MainView::fullScreen);
    this->view_menu->addMenu("&Text Encoding");
    this->view_menu->addMenu("&Page Style");
    this->view_menu->addAction("&Reset View");
    this->history_menu=this->menubar->addMenu("&History");
    this->show_all_history=this->history_menu->addAction("&Show All History");
    this->history_menu->addAction("&Clear Recent History");
    this->history_menu->addAction("&Manage History");
    this->history_menu->addSeparator();
    this->history_menu->addAction("&Restore Previous Session");
    this->recently_closed=this->history_menu->addMenu("&Recently Closed");
    this->history_menu->addMenu("&Most Visited");
    this->bookmark_menu=this->menubar->addMenu("&Bookmarks");
    this->bookmark_menu->addAction("&Bookmark This Page");
    this->bookmark_menu->addAction("&Bookmark All Tabs");
    this->bookmark_menu->addAction("&Show All Bookmarks");
    this->bookmark_menu->addAction("&Manage Bookmarks");
    this->bookmark_menu->addSeparator();
    this->bookmark_menu->addMenu("&Recent Bookmarks");
    this->bookmark_menu->addMenu("&Crusta Bookmarks");
    this->download_menu=this->menubar->addMenu("&Downloads");
    this->download_menu->addAction("&Download Manager");
    this->download_menu->addAction("&Clear all Downloads");
    this->tool_menu=this->menubar->addMenu("&Tools");
    this->tool_menu->addAction("&Site Info");
    this->tool_menu->addAction("&Crusta Speak");
    this->tool_menu->addSeparator();
    this->tool_menu->addAction("&Cookies Manager");
    this->web_inspector_action=this->tool_menu->addAction("&Web Inspector");
    this->tool_menu->addSeparator();
    this->devTools=this->tool_menu->addMenu("&Developer Tools");
    this->runJsCode=this->devTools->addAction("&Run Javascript Code");
    connect(this->runJsCode,&QAction::triggered,this,&MainView::showJsCodeEditor);
    this->help_menu=this->menubar->addMenu("&Help");
    this->help_menu->addAction("&Crusta Help");
    this->help_menu->addAction("&Crusta Tour");
    this->aboutCr=this->help_menu->addAction("&About Crusta");
    connect(this->aboutCr,&QAction::triggered,this,&MainView::about);
    this->help_menu->addSeparator();
    this->help_menu->addAction("&Keyboard Shortcuts");
    this->help_menu->addAction("&Request Feature");
    this->help_menu->addAction("&Report Issue");
    this->help_menu->addSeparator();
    this->help_menu->addAction("&License");
    this->box->setMenuBar(this->menubar);
    this->menubar->setNativeMenuBar(true);
}

void MainView::createTabWindow(){
    this->tabWindow->setMovable(true);
    this->tabWindow->setTabsClosable(true);
    this->tabWindow->setContentsMargins(0,0,0,0);
    this->box->addWidget(this->tabWindow);
}

void MainView::addNormalTab(){
    TabWindow* tab=new TabWindow();
    this->tabWindow->addTab(tab->returnTab(),"new Tab");
    this->tabWindow->setCurrentIndex(this->tabWindow->count()-1);
    MainView::addNewTabButton();
}

void MainView::addPrivateTab(){
    PrivateTabWindow* privatetab=new PrivateTabWindow();
    this->tabWindow->addTab(privatetab->returnTab(),"new Tab");
    this->tabWindow->setCurrentIndex(this->tabWindow->count()-1);
    MainView::addNewTabButton();
}

void MainView::viewPageSource(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    QString qurl=webview->url().toString();
    qurl=QString("view-source:")+qurl;
//    QWebEngineView* w=new QWebEngineView();
//    w->load(QUrl(qurl));
//    w->show();
//    w->setWindowTitle(QString("Crusta : ")+webview->title());
    TabWindow* tab=new TabWindow();
    index++;
    this->tabWindow->insertTab(index,tab->returnTab(),"new Tab");
    this->tabWindow->setCurrentIndex(index);
    QWidget* wid=this->tabWindow->widget(index);
    QLayout* lay=wid->layout();
    QWebEngineView* webv=(QWebEngineView*)lay->itemAt(1)->widget();
    webv->load(QUrl(qurl));
    MainView::addNewTabButton();
}

void MainView::saveAsPdf(){
    QPrinter printer;
    printer.setPageLayout(currentPageLayout);
    QPageSetupDialog dlg(&printer, this);
    if (dlg.exec() != QDialog::Accepted)
        return;
    currentPageLayout.setPageSize(printer.pageLayout().pageSize());
    currentPageLayout.setOrientation(printer.pageLayout().orientation());
    QFileDialog f;
    f.setOption(QFileDialog::DontUseNativeDialog,true);
    QString file_name=f.getSaveFileName(this->window,"Crusta : Save File",QDir::homePath(),"Pdf File(*.pdf)",nullptr,f.options());
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    if(file_name!=""){
    if(!file_name.endsWith(".pdf"))file_name+=QString(".pdf");
    webview->page()->printToPdf(file_name,QPageLayout(printer.pageLayout().pageSize(),printer.pageLayout().orientation(),printer.pageLayout().margins()));
    }
}

void MainView::savePage(){
    QFileDialog f;
    f.setOption(QFileDialog::DontUseNativeDialog,true);
    QString file_name=f.getSaveFileName(this->window,"Crusta : Save File",QDir::homePath(),"WebPage, Complete",nullptr,f.options());
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    if(file_name!=QString("")){
        webview->page()->save(file_name,QWebEngineDownloadItem::CompleteHtmlSaveFormat);
    }
}

void MainView::showJsCodeEditor(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    jsEditor->setView(webview);
    jsEditor->show();
}

void MainView::openLocalFile(){
    QFileDialog f;
    f.setOption(QFileDialog::DontUseNativeDialog,true);
    QString filename=f.getOpenFileName(this->window,"Crusta : Open File",QDir::homePath(),QString(),nullptr,f.options());
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    if(filename!=""){
    webview->load(QUrl(QString("file://")+filename));
    }
}

void MainView::screenShot(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    QPixmap pmap = webview->grab();
    QFileDialog f;
    f.setOption(QFileDialog::DontUseNativeDialog,true);
    QString filename=f.getSaveFileName(this->window,"Crusta : Open File",QDir::homePath(),QString("Images (*.png *.xpm *.jpg *.bmp)"),nullptr,f.options());
    if(filename!=""){
    if(!(filename.endsWith(".png")||filename.endsWith(".jpg")||filename.endsWith(".bmp")||filename.endsWith(".xpm")))filename+=QString(".png");
    pmap.save(filename);
    }
}

void MainView::tabAreaDoubleClicked(int index){
    if(index==-1){
        MainView::addNormalTab();
    }
}

void MainView::addNewTabButton(){
    int cnt=this->tabWindow->count();
    int x=cnt*175+5; //size of a tab;
    if(newtabbtn->parent()==NULL)newtabbtn->setParent(this->tabWindow->tabBar());
    this->newtabbtn->move(x,5);
    this->newtabbtn->setFixedHeight(22);
    this->newtabbtn->setFixedWidth(25);
}

void MainView::editPreference(){
    ThemeEditor* th=new ThemeEditor();
    th->setWindowTitle("Theme Editor");
    th->show();
}

void MainView::duplicateTab(QWebEngineView* view){
    TabWindow* tab=new TabWindow();
    WebView* wview=new WebView();
    wview->load(view->url());
    this->tabWindow->addTab(tab->returnTab(wview),"new Tab");
    this->tabWindow->setCurrentIndex(this->tabWindow->count()-1);
    MainView::addNewTabButton();
}

void MainView::reloadAllTabs(){
    int cnt=this->tabWindow->count();
    for(int i=0;i<cnt;i++){
        QWidget* widget=this->tabWindow->widget(i);
        QLayout* layout=widget->layout();
        QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
        webview->reload();
    }
}

void MainView::closeOtherTabs(int index){
    int cnt=this->tabWindow->count();
    int i=0;
    while(i<cnt){
        if(i!=index){
            QWidget* widget=this->tabWindow->widget(i);
            QLayout* layout=widget->layout();
            QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
            webview->page()->windowCloseRequested();
            if(index>0)index--;
            cnt--;
            i=0;
            MainView::addNewTabButton();
            continue;
        }
        i++;
    }
}

void MainView::restoreTab(QUrl u){
    MainView::addNormalTab();
    int index=this->tabWindow->count()-1;
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->load(u);
}

void MainView::about(){
    MainView::addNormalTab();
    int index=this->tabWindow->count()-1;
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->load(QUrl("http://www.crustabrowser.com/about"));
}
