#pragma once

#include <QAction>
#include <QWidget>

class ToolBarPopup : public QWidget
{
public:
    enum Position
    {
        Leading,
        Trailing
    };
    explicit ToolBarPopup(QWidget *parent = nullptr);
    void setParentAction(QAction *action);
    QAction *parentAction();

    void show();

    Position position = Leading;
private:
    QAction *m_parentAction = nullptr;
};
