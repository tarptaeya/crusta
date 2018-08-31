#pragma once

#include "../extra/qclickablelabel.h"
#include <QWidget>
#include <QHBoxLayout>

class TabWidget;
class TabBarButton;
class OmniBar;
class Tab;

class TabBar : public QWidget
{
public:
    explicit TabBar(QWidget *parent = nullptr);
    void setVirtualTab(Tab *tab);
private:
    TabWidget *m_tabWidget = nullptr;
    QHBoxLayout *m_hBoxLayout = nullptr;

    TabBarButton *m_backButton = nullptr;
    TabBarButton *m_forwardButton = nullptr;
    TabBarButton *m_stopReloadButton = nullptr;
    TabBarButton *m_homeButton = nullptr;
    TabBarButton *m_newTabButton = nullptr;
    TabBarButton *m_tabListButton = nullptr;
    QClickableLabel *m_loadingTimeLabel = nullptr;
    TabBarButton *m_optionsButton = nullptr;

    OmniBar *m_omniBar = nullptr;

    Tab *m_virtualTab = nullptr;

    QList<QMetaObject::Connection> m_connections;

    void createVirtualConnections();
    void destroyVirtualConnections();
};
