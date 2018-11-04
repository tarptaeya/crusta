#pragma once

#include <QAction>
#include <QWidget>

class ToolBarPopup : public QWidget
{
public:
    explicit ToolBarPopup(QWidget *parent = nullptr);
    void setParentAction(QAction *action);
    QAction *parentAction();
private:
    QAction *m_parentAction = nullptr;
};
