#pragma once

#include <QStackedWidget>

class Tab;

class StackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit StackedWidget(QWidget *parent = nullptr);
    int addWidget(QWidget *widget);
private:
    QList<Tab *> m_tabWidgetList;
Q_SIGNALS:
    void widgetAdded(QWidget *widget);
};
