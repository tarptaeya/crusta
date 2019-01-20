#ifndef CR_MENU_H
#define CR_MENU_H

#include <QMenu>

class Menu : public QMenu
{
public:
    explicit Menu(QWidget *parent = nullptr);

private:
    void setUpMenu();
};

#endif
