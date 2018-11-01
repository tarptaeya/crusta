#pragma once

#include <QObject>
#include <QToolButton>

class ToolBarButton : public QToolButton
{
public:
    explicit ToolBarButton(QWidget *parent = nullptr);
    void setIconFromFileName(const QString &iconFileName);
};
