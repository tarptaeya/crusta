#pragma once

#include <QStackedWidget>
#include <QTabBar>
#include <QToolBar>
#include <QToolButton>

class Tab;
class BrowserTabbar;

class CentralWidget : public QWidget
{
    QStackedWidget *m_stacked_widget = nullptr;
    BrowserTabbar *m_tabbar = nullptr;
    QToolBar *m_toolbar = nullptr;

    void setup_tabbar();
    void setup_toolbar();

public:
    explicit CentralWidget(QWidget *parent = nullptr);

    Tab *add_new_tab();
};

class BrowserTabbar : public QTabBar
{
    Q_OBJECT

    QToolButton *m_add_tab_button = nullptr;
public:
    explicit BrowserTabbar(QWidget *parent = nullptr);

    QSize tabSizeHint(int index) const override;

signals:
    void new_tab_requested();
};
