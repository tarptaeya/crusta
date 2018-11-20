#include "toolbarpopup.h"
#include <QApplication>

ToolBarPopup::ToolBarPopup(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlag(Qt::Popup);
}

void ToolBarPopup::setParentAction(QAction *action)
{
    m_parentAction = action;
}

QAction *ToolBarPopup::parentAction()
{
    return m_parentAction;
}

void ToolBarPopup::show()
{
    QWidget::show();

    QWidget *parentWidget = ToolBarPopup::parentAction()->parentWidget();
    QPoint point;
    bool showLeading = qApp->layoutDirection() == Qt::LeftToRight && position == Leading;
    showLeading = showLeading || (qApp->layoutDirection() == Qt::RightToLeft && position == Trailing);
    if (showLeading) {
        point = parentWidget->mapToGlobal(parentWidget->rect().bottomLeft());
    } else {
        point = parentWidget->mapToGlobal(parentWidget->rect().bottomRight());
        point.setX(point.x() - rect().width());
    }
    move(point);
}
