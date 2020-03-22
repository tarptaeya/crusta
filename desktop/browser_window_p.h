#pragma once

#include <QStackedWidget>
#include <QTabBar>
#include <QToolButton>

class Tab;
class NormalTabbar;

class CentralWidget : public QWidget
{
    QStackedWidget *m_stacked_widget = nullptr;
    NormalTabbar *m_tabbar = nullptr;

    void setup_tabbar();

public:
    explicit CentralWidget(QWidget *parent = nullptr);

    Tab *add_new_tab();
    QList<Tab *> tabs() const;
};

class NormalTabbar : public QTabBar
{
    Q_OBJECT

    QToolButton *m_add_tab_button = nullptr;
public:
    explicit NormalTabbar(QWidget *parent = nullptr);

    QSize tabSizeHint(int index) const override;

signals:
    void new_tab_requested();
};
