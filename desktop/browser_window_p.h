#pragma once

#include <QStackedWidget>
#include <QTabBar>
#include <QToolBar>
#include <QToolButton>

class CentralWidget : public QWidget
{
    QStackedWidget *m_stacked_widget = nullptr;
    QTabBar *m_tabbar = nullptr;
    QToolBar *m_toolbar = nullptr;

    void setup_tabbar();
    void setup_toolbar();

public:
    explicit CentralWidget(QWidget *parent = nullptr);
};

class BrowserTabbar : public QTabBar
{
    Q_OBJECT

    QToolButton *m_add_tab_button = nullptr;
public:
    explicit BrowserTabbar(QWidget *parent = nullptr);

    QSize tabSizeHint(int index) const override;

Q_SIGNALS:
    void new_tab_requested();
};
