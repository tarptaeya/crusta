#pragma once

#include <QWidget>
#include <QVBoxLayout>

class Tab;
class TabWidget;

class TabList : public QWidget
{
    Q_OBJECT
public:
    explicit TabList(QWidget *parent = nullptr);
    void show();
    void addTab(Tab *tab);
    void closeTab(Tab *tab);
    void setVirtualTabWidget(TabWidget *tabWidget);
    int indexOf(Tab *tab);
private:
    int m_preferedWidth = 0;
    int m_tabCount = 0;
    QVBoxLayout *m_vBoxLayout = nullptr;
    TabWidget *m_tabWidget = nullptr;
};
