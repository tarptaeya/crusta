#pragma once

#include <QWidget>
#include <QHBoxLayout>

class TabBarButton;
class OmniBar;

class TabBar : public QWidget
{
public:
    explicit TabBar(QWidget *parent = nullptr);
private:
    QHBoxLayout *m_hBoxLayout = nullptr;

    TabBarButton *m_backButton = nullptr;
    TabBarButton *m_forwardButton = nullptr;
    TabBarButton *m_homeButton = nullptr;
    TabBarButton *m_newTabButton = nullptr;
    TabBarButton *m_tabListButton = nullptr;
    TabBarButton *m_optionsButton = nullptr;

    OmniBar *m_omniBar = nullptr;
};
