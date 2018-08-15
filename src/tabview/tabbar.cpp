#include "tabbar.h"
#include "tabbarbutton.h"
#include "omnibar/omnibar.h"
#include "../utils/dimensions.h"

TabBar::TabBar(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setFixedHeight(Dimensions::tabBarHeight());

    m_hBoxLayout = new QHBoxLayout(this);
    setLayout(m_hBoxLayout);
    m_hBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_hBoxLayout->setSpacing(0);

    m_backButton = new TabBarButton(this);
    m_forwardButton = new TabBarButton(this);
    m_homeButton = new TabBarButton(this);
    m_newTabButton = new TabBarButton(this);
    m_tabListButton = new TabBarButton(this);
    m_optionsButton = new TabBarButton(this);

    m_omniBar = new OmniBar(this);

    m_hBoxLayout->addWidget(m_backButton);
    m_hBoxLayout->addWidget(m_forwardButton);
    m_hBoxLayout->addWidget(m_homeButton);
    m_hBoxLayout->addWidget(m_omniBar);
    m_hBoxLayout->addWidget(m_newTabButton);
    m_hBoxLayout->addWidget(m_tabListButton);
    m_hBoxLayout->addWidget(m_optionsButton);
}
