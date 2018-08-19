#pragma once

#include <QStackedWidget>

class Tab;

class StackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit StackedWidget(QWidget *parent = nullptr);
    int addWidget(Tab *tab);
private:
    QList<Tab *> m_tabWidgetList;
Q_SIGNALS:
    void widgetAdded(Tab *tab);
};
