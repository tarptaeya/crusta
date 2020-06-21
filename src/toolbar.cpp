#include "toolbar.h"
#include "addressbar.h"

ToolBar::ToolBar(QWidget *parent)
    : QToolBar(parent)
{
    m_addressBar = new AddressBar;
    addWidget(m_addressBar);
}
