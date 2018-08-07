#pragma once

#include <QWidget>

class TabView : public QWidget
{
    Q_OBJECT
public:
    explicit TabView(QWidget *parent = nullptr);
    ~TabView();
};
