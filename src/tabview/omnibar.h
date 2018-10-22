#pragma once

#include <QLineEdit>
#include <QAction>
#include <QUrl>

class OmniBar : public QLineEdit
{
public:
    explicit OmniBar(QWidget *parent = nullptr);
    void setAddress(const QUrl &address);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
private:
    QUrl m_address;
    QAction *m_siteInfoAction = nullptr;
    QAction *m_bookmarkPageAction = nullptr;
};
