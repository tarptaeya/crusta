#ifndef ADDRESSLINEEDIT_H
#define ADDRESSLINEEDIT_H

#include <QLineEdit>
#include <QPushButton>
#include <QIcon>



class AddressLineEdit{
    // TODO :: ADD COMPLETER TO IT
public:
    QLineEdit* addr_bar=new QLineEdit();
    QPushButton* info_btn=new QPushButton();
    QPushButton* secure_btn=new QPushButton();
    void createAddressLineEdit();
    QLineEdit* initialize();
};



#endif // ADDRESSLINEEDIT_H
