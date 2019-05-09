#pragma once

#include <QComboBox>
#include <QStackedWidget>
#include <QWidget>

class SideBar : public QWidget
{
public:
    explicit SideBar(QWidget *parent = nullptr);
    void addItem(const QString &title, QWidget *widget);

private:
    QComboBox *m_comboBox = nullptr;
    QStackedWidget *m_widget = nullptr;
};
