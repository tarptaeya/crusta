#include "manager.h"
#include "historymanager.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QTabWidget>
#include <QTabBar>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QListView>
#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>
#include <QIcon>
#include <QSize>




void Manager::createManager(){
    this->createOptions();
    this->createPages();
    this->setLayout(this->hbox);
    this->hbox->addWidget(this->list);
    this->hbox->addWidget(this->stack);
    this->setFixedHeight(500);
    this->setFixedWidth(800);
    this->move(QApplication::desktop()->screenGeometry().center()-this->rect().center());
    this->setWindowTitle("Crusta Manager");
}

void Manager::createOptions(){
    this->list=new QListWidget();
    this->list->setViewMode(QListView::IconMode);
    this->list->setIconSize(QSize(96, 84));
    this->list->setFixedWidth(110);
    this->list->setSpacing(15);
    QListWidgetItem* history=new QListWidgetItem(this->list);
    QListWidgetItem* bookmarks=new QListWidgetItem(this->list);
    QListWidgetItem* downloads=new QListWidgetItem(this->list);
    QListWidgetItem* password=new QListWidgetItem(this->list);
    QListWidgetItem* settings=new QListWidgetItem(this->list);
    history->setIcon(QIcon(":/res/manager/history.png"));
    history->setText("History");
    history->setTextAlignment(Qt::AlignHCenter);
    history->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    bookmarks->setIcon(QIcon(":/res/manager/bookmarks.png"));
    bookmarks->setText("Bookmarks");
    bookmarks->setTextAlignment(Qt::AlignHCenter);
    bookmarks->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    downloads->setIcon(QIcon(":/res/manager/download.png"));
    downloads->setText("Downloads");
    downloads->setTextAlignment(Qt::AlignHCenter);
    downloads->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    password->setIcon(QIcon(":/res/manager/password.png"));
    password->setText("Passwords");
    password->setTextAlignment(Qt::AlignHCenter);
    password->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    settings->setIcon(QIcon(":/res/manager/settings.png"));
    settings->setText("Settings");
    settings->setTextAlignment(Qt::AlignHCenter);
    settings->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void Manager::createPages(){
    this->stack=new QStackedWidget();
    history_page->createManager();
    stack->addWidget(history_page);
}

void Manager::clearHistoryPage(){
    HistoryManager* history_page=(HistoryManager*)stack->widget(0);
    history_page->treeview->clear();
}
