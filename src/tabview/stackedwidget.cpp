#include "stackedwidget.h"
#include "tab.h"
#include <QDebug>

StackedWidget::StackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    connect(this, &StackedWidget::widgetAdded, this, [this](QWidget *widget){
        Tab *tab = qobject_cast<Tab *>(widget);
        if (!tab) {
            qWarning() << "Unable to cast widget to TabWidget";
            return;
        }
        m_tabWidgetList.append(tab);
    });
}

int StackedWidget::addWidget(QWidget *widget)
{
    int index = QStackedWidget::addWidget(widget);
    if (index == -1) {
        qWarning() << "Adding widget to StackedWidget failed";
        return index;
    }
    emit widgetAdded(widget);
    return index;
}
