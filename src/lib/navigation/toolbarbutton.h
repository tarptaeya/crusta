#ifndef CR_TOOL_BAR_BUTTON_H
#define CR_TOOL_BAR_BUTTON_H

#include <QLabel>
#include <QToolButton>
#include <QVariant>

class ToolBarButton : public QToolButton
{
public:
    explicit ToolBarButton(QWidget *parent = nullptr);

    void setBadgeCount(int count);

    QVariant data() const;
    void setData(const QVariant &data);
private:
    QLabel *m_badgeLabel = nullptr;
    QVariant m_data;
};

#endif
