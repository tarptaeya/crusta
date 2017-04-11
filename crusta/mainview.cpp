#include "mainview.h"

#include "fullscreennotifier.h"
#include "webview.h"
#include "searchlineedit.h"
#include "addresslineedit.h"
#include "tabwindow.h"
#include "presentationmodenotifier.h"
#include "manager.h"

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
#include <QWebEngineDownloadItem>




void MainView::closeTab(int index){
    if(this->tabWindow->count()==1)QApplication::quit();
    QWidget* widget=this->tabWindow->widget(index);
    widget->deleteLater();
    this->tabWindow->removeTab(index);
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
        this->fullscreen_action->setIcon(QIcon());
    }
    else{
        this->window->showFullScreen();
        this->fullscreen_action->setIcon(QIcon(":/res/menu/tick.png"));
    }
}


void MainView::tabBarContext(QPoint point){
    if(this->tabWindow->tabBar()->tabAt(point)!=-1){
        QMenu* contextMenu=new QMenu();
        contextMenu->addAction("&Reload Tab");
        contextMenu->addAction("&Mute Tab");
        contextMenu->addAction("&Pin Tab");
        contextMenu->addAction("&Duplicate Tab");
        contextMenu->addAction("&Open in new Window");
        contextMenu->addAction("&Bookmark Tab");
        contextMenu->addAction("&Bookmark All Tabs");
        contextMenu->addAction("&Close Other Tabs");
        contextMenu->exec(contextMenu->mapToGlobal(point));
    }
    else{
        QMenu* barContext=new QMenu();
        barContext->addAction("&New Tab");
        barContext->addAction("&Reload All Tabs");
        barContext->addAction("&Bookmark All Tabs");
        barContext->addAction("Mute All Tabs");
        barContext->addAction("&Restore All Tabs");
        barContext->exec(barContext->mapToGlobal(point));
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
    this->tabWindow->setStyleSheet("QTabBar::tab{max-width:175px;min-width:175px}");
    this->tabWindow->tabBar()->setExpanding(false);
    this->tabWindow->tabBar()->setShape(QTabBar::RoundedNorth);
    this->tabWindow->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->tabWindow->tabBar(),&QTabBar::tabCloseRequested,this,&MainView::closeTab);
    connect(this->tabWindow->tabBar(),&QTabBar::customContextMenuRequested,this,&MainView::tabBarContext);
    createView();
    createMenuBar();
    createTabWindow();
    addNormalTab();
    this->manager->createManager();
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
    this->file_menu->addAction("&New Private Tab");
    this->file_menu->addAction("&New Private Window");
    this->file_menu->addAction("&Open File");
    this->save_as_pdf=this->file_menu->addAction("&Save Page As PDF");
    connect(this->save_as_pdf,&QAction::triggered,this,&MainView::saveAsPdf);
    this->save_page=this->file_menu->addAction("&Save Page");
    connect(this->save_page,&QAction::triggered,this,&MainView::savePage);
    this->file_menu->addAction("&Send Link");
    this->file_menu->addAction("&Print");
    this->exit_action=this->file_menu->addAction("&Quit");
    connect(this->exit_action,&QAction::triggered,this,&MainView::quit);
    this->edit_menu=this->menubar->addMenu("&Edit");
    this->undo_action=this->edit_menu->addAction("&Undo");
    connect(this->undo_action,&QAction::triggered,this,&MainView::undoPageAction);
    this->redo_action=this->edit_menu->addAction("&Redo");
    connect(this->redo_action,&QAction::triggered,this,&MainView::redoPageAction);
    this->cut_action=this->edit_menu->addAction("&Cut");
    connect(this->cut_action,&QAction::triggered,this,&MainView::cutPageAction);
    this->copy_action=this->edit_menu->addAction("&Copy");
    connect(this->copy_action,&QAction::triggered,this,&MainView::copyPageAction);
    this->paste_action=this->edit_menu->addAction("&Paste");
    connect(this->paste_action,&QAction::triggered,this,&MainView::pastePageAction);
    this->selectall_action=this->edit_menu->addAction("&Select All");
    connect(this->selectall_action,&QAction::triggered,this,&MainView::selectAllText);
    this->find_action=this->edit_menu->addAction("&Find");
    this->find_action->setShortcut(QKeySequence(QKeySequence::Find));
    connect(this->find_action,&QAction::triggered,this,&MainView::FindText);
    this->edit_menu->addAction("&Edit Preference");
    this->view_menu=this->menubar->addMenu("&View");
    this->view_menu->addMenu("&ToolBar");
    this->view_menu->addMenu("&SideBar");
    this->view_menu->addAction("&Status Bar");
    this->view_page_source_action=this->view_menu->addAction("&Page Source");
    connect(this->view_page_source_action,&QAction::triggered,this,&MainView::viewPageSource);
    this->zoom_in_action=this->view_menu->addAction("&Zoom In");
    connect(this->zoom_in_action,&QAction::triggered,this,&MainView::zoomIn);
    this->zoom_out_action=this->view_menu->addAction("&Zoom Out");
    connect(this->zoom_out_action,&QAction::triggered,this,&MainView::zoomOut);
    this->reset_zoom_action=this->view_menu->addAction("&Reset Zoom");
    connect(this->reset_zoom_action,&QAction::triggered,this,&MainView::resetZoom);
    this->presentation_action=this->view_menu->addAction("&Presentation Mode");
    connect(this->presentation_action,&QAction::triggered,this,&MainView::enterPresentationMode);
    this->fullscreen_action=this->view_menu->addAction("&Full Screen");
    connect(this->fullscreen_action,&QAction::triggered,this,&MainView::fullScreen);
    this->view_menu->addMenu("&Text Encoding");
    this->view_menu->addMenu("&Page Style");
    this->view_menu->addAction("&Reset View");
    this->history_menu=this->menubar->addMenu("&History");
    this->show_all_history=this->history_menu->addAction("&Show All History");
    connect(this->show_all_history,&QAction::triggered,this,&MainView::showManager);
    this->history_menu->addAction("&Clear Recent History");
    this->history_menu->addAction("&Manage History");
    this->history_menu->addAction("&Restore Previous Session");
    this->history_menu->addMenu("&Recently Closed");
    this->history_menu->addMenu("&Most Visited");
    this->bookmark_menu=this->menubar->addMenu("&Bookmarks");
    this->bookmark_menu->addAction("&Bookmark This Page");
    this->bookmark_menu->addAction("&Bookmark All Tabs");
    this->bookmark_menu->addAction("&Show All Bookmarks");
    this->bookmark_menu->addAction("&Manage Bookmarks");
    this->bookmark_menu->addMenu("&Recent Bookmarks");
    this->bookmark_menu->addMenu("&Crusta Bookmarks");
    this->tool_menu=this->menubar->addMenu("&Tools");
    this->tool_menu->addAction("&Site Info");
    this->tool_menu->addAction("&Crusta Speak");
    this->tool_menu->addAction("&Download Manager");
    this->tool_menu->addAction("&Cookies Manager");
    this->web_inspector_action=this->tool_menu->addAction("&Web Inspector");
    connect(this->web_inspector_action,&QAction::triggered,this,&MainView::showInspector);
    this->tool_menu->addMenu("&Developer Tools");
    this->help_menu=this->menubar->addMenu("&Help");
    this->help_menu->addAction("&Crusta Help");
    this->help_menu->addAction("&Crusta Tour");
    this->help_menu->addAction("&About Crusta");
    this->help_menu->addAction("&Keyboard Shortcuts");
    this->help_menu->addAction("&Request Feature");
    this->help_menu->addAction("&Report Issue");
    this->help_menu->addAction("&License");
    this->box->setMenuBar(this->menubar);
    this->menubar->setNativeMenuBar(false);
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
}

void MainView::viewPageSource(){
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    QString qurl=webview->url().toString();
    qurl=QString("view-source:")+qurl;
    QWebEngineView* w=new QWebEngineView();
    w->load(QUrl(qurl));
    w->show();
    w->setWindowTitle(QString("Crusta : ")+webview->title());
}

void MainView::saveAsPdf(){
    QString file_name=QFileDialog::getSaveFileName(this->window,"Crusta : Save File",QDir::homePath(),"Pdf File(*.pdf)");
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->page()->printToPdf(file_name);
}

void MainView::savePage(){
    QString file_name=QFileDialog::getSaveFileName(this->window,"Crusta : Save File",QDir::homePath(),"WebPage, Complete");
    int index=this->tabWindow->currentIndex();
    QWidget* widget=this->tabWindow->widget(index);
    QLayout* layout=widget->layout();
    QWebEngineView* webview=(QWebEngineView*)layout->itemAt(1)->widget();
    webview->page()->save(file_name,QWebEngineDownloadItem::CompleteHtmlSaveFormat);
}

void MainView::showManager(){
    manager=new Manager();
    manager->createManager();
    manager->show();
}

void MainView::showInspector(){
    QWebEngineView* new_view=new QWebEngineView();
    new_view->load(QUrl("http://localhost:8000"));
    new_view->show();
    new_view->setWindowTitle("Crusta DevConsole");
}








