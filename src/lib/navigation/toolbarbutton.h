#ifndef CR_TOOL_BAR_BUTTON_H
#define CR_TOOL_BAR_BUTTON_H

#include <QLabel>
#include <QToolButton>

class ToolBarButton : public QToolButton
{
public:
    explicit ToolBarButton(QWidget *parent = nullptr);

    void setBadgeCount(int count);
private:
    QLabel *m_badgeLabel = nullptr;
};

#endif
