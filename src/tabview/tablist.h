#pragma once

#include <QWidget>

class TabList : public QWidget
{
    Q_OBJECT
public:
    explicit TabList(QWidget *parent = nullptr);
    void show();
private:
    int m_preferedWidth = 0;
};
