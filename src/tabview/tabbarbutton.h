#pragma once

#include <QPushButton>
#include <QVariant>

class TabBarButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TabBarButton(QWidget *parent);
    QVariant data() const;
    void setData(QVariant data);
private:
    QVariant m_data;
};
