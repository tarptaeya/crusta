#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>

class AddressBar;

class ToolBar : public QToolBar
{
    AddressBar *m_addressBar = nullptr;
public:
    ToolBar(QWidget *parent = nullptr);
};

#endif // TOOLBAR_H
