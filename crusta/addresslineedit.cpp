#include <addresslineedit.h>
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>


void AddressLineEdit::createAddressLineEdit(){
    this->addr_bar->setTextMargins(45,0,0,0);
    this->info_btn->setIcon(QIcon(":/res/drawables/info.png"));
    this->info_btn->setFlat(true);
    this->info_btn->setParent(this->addr_bar);
    this->secure_btn->setIcon(QIcon(":/res/drawables/secure.png"));
    this->secure_btn->setFlat(true);
    this->secure_btn->setParent(this->addr_bar);
    this->secure_btn->move(20,0);
}

QLineEdit* AddressLineEdit::initialize(){
    createAddressLineEdit();
    return this->addr_bar;
}
