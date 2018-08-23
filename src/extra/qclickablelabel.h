#pragma once

#include <QLabel>

class QClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QClickableLabel(QWidget *parent = nullptr);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
Q_SIGNALS:
    void clicked();
    void doubleClicked();
};
