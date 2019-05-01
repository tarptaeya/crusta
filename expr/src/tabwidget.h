#pragma once

#include <QToolButton>
#include <QTabWidget>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);

Q_SIGNALS:
    void windowCloseRequested();

private:
    QToolButton *m_newTabButton = nullptr;
};
