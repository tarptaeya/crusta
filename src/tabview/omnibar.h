#pragma once

#include <QLineEdit>
#include <QAction>

class OmniBar : public QLineEdit
{
public:
    explicit OmniBar(QWidget *parent = nullptr);
private:
    QAction *m_siteInfoAction = nullptr;
    QAction *m_bookmarkPageAction = nullptr;
};
