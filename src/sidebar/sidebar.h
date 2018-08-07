#pragma once

#include <QWidget>

class SideBar : public QWidget
{
    Q_OBJECT
public:
    enum State {
        Free,
        Docked
    };

    explicit SideBar(QWidget *parent = nullptr);
    ~SideBar();
};
