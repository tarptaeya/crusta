#ifndef CR_MENU_H
#define CR_MENU_H

#include "historyitem.h"

#include <QMenu>

class Menu : public QMenu
{
public:
    explicit Menu(QWidget *parent = nullptr);

    static QList<HistoryItem> s_historyItems;

private:
    void setUpMenu();
};

#endif
