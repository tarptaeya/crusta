#include "stackedwidget.h"
#include "tab.h"
#include <QDebug>

StackedWidget::StackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    connect(this, &StackedWidget::widgetAdded, this, [this](Tab *tab){
        m_tabWidgetList.append(tab);
    });
}

int StackedWidget::addWidget(Tab *tab)
{
    int index = QStackedWidget::addWidget(tab);
    if (index == -1) {
        qWarning() << "Adding widget to StackedWidget failed";
        return index;
    }
    emit widgetAdded(tab);
    return index;
}
