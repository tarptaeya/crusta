#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QLineEdit>
#include <QPushButton>
#include <QMenu>
#include <QIcon>

class SearchLineEdit{
public:
    QLineEdit* text=new QLineEdit();
    QPushButton* search_btn=new QPushButton();
    QMenu* menu=new QMenu();
    void createSearchLineEdit();
    void createSearchOptions();
    QLineEdit* initialize();
};



#endif // SEARCHLINEEDIT_H
