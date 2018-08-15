#pragma once

#include <QWidget>

class TabBar;
class StackedWidget;

class TabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);
    ~TabWidget();
private:
    TabBar *m_tabBar = nullptr;
    StackedWidget *m_stakedWidget = nullptr;
};
