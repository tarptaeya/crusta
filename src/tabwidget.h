#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QToolButton>

class Tab;

class TabWidget : public QTabWidget
{
    QToolButton *m_newTabButton = nullptr;
public:
    TabWidget(QWidget *parent = nullptr);

    Tab *newTab();
};

#endif // TABWIDGET_H
