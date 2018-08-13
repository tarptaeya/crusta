#pragma once

#include <QSplitter>

class SideBar;
class TabView;

class CentralWidget : public QSplitter
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();
private:
    SideBar *m_sideBar = nullptr;
};
