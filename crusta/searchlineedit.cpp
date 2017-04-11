#include <searchlineedit.h>
#include <QPushButton>
#include <QMenu>
#include <QLineEdit>
#include <QIcon>

void SearchLineEdit::createSearchLineEdit(){
    this->text->setTextMargins(37,0,0,0);
    this->text->setStyleSheet("max-width:170px");
    createSearchOptions();
}

void SearchLineEdit::createSearchOptions(){
    this->menu->addAction(QIcon(":/res/favicons/google.png"),"&Google");
    this->menu->addAction(QIcon(":/res/favicons/bing.png"),"&Bing");
    this->menu->addAction(QIcon(":/res/favicons/yahoo.png"),"&Yahoo");
    this->menu->addAction(QIcon(":/res/favicons/duckduckgo.png"),"&DuckDuckGo");
    this->search_btn->setMenu(this->menu);
    this->search_btn->setIcon(QIcon(":/res/drawables/search.png"));
    this->search_btn->setParent(this->text);
    this->search_btn->setFlat(true);
}

QLineEdit* SearchLineEdit::initialize(){
    createSearchLineEdit();
    return this->text;
}
