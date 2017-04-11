#ifndef MANAGER_H
#define MANAGER_H

#include "historymanager.h"

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QStackedWidget>




class Manager:public QWidget{
public:
    HistoryManager* history_page=new HistoryManager();
    QHBoxLayout* hbox=new QHBoxLayout();
    QListWidget* list;
    QStackedWidget* stack;
    void createManager();
    void createOptions();
    void createPages();
    void clearHistoryPage();
};




#endif // MANAGER_H
