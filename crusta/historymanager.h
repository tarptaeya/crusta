#ifndef HISTORYMANAGER_H
#define HISTORYMANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTreeWidget>




class HistoryManager:public QWidget{
public:
    QVBoxLayout* vbox=new QVBoxLayout();
    QLineEdit* searchline=new QLineEdit();
    QPushButton* del_btn=new QPushButton();
    QPushButton* clear_all=new QPushButton();
    QComboBox* time=new QComboBox();
    QHBoxLayout* hbox=new QHBoxLayout();
    QHBoxLayout* h1box=new QHBoxLayout();
    QHBoxLayout* h2box=new QHBoxLayout();
    QLabel* lbl=new QLabel();
    QTreeWidget* treeview=new QTreeWidget();
    void createManager();
    void createHeader();
    void retrieveHistory();
};





#endif // HISTORYMANAGER_H
