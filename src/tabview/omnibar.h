#pragma once

#include <QLineEdit>
#include <QAction>
#include <QUrl>

class ToolBar;

class OmniBar : public QLineEdit
{
public:
    explicit OmniBar(QWidget *parent = nullptr);
    void setAddress(const QUrl &address);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void updateBookmarksIcon(bool isBookmarked);

    void navigate();
private:
    QUrl m_address;
    QAction *m_siteInfoAction = nullptr;
    QAction *m_bookmarkPageAction = nullptr;
    ToolBar *m_parent = nullptr;
};
