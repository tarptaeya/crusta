#pragma once

#include "../extra/qclickablelabel.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class Tab;
class TabWidget;

class TabListItem : public QWidget
{
    Q_OBJECT
    enum State {
        Normal,
        Current,
        Hovered
    };
public:
    explicit TabListItem(QWidget *parent = nullptr);
    void setVirtualTab(Tab *tab);
    void setVirtualTabWidget(TabWidget *tabWidget);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
    void setCurrent(bool current);
private:
    State m_state = Normal;
    QHBoxLayout *m_hBoxLayout = nullptr;
    Tab *m_tab = nullptr;
    TabWidget *m_tabWidget = nullptr;
    QLabel *m_favicon = nullptr;
    QLabel *m_title = nullptr;
    QClickableLabel *m_closeTabButton = nullptr;

    void setFavicon(const QIcon &favicon);
    void setTitle(const QString &title);
    void showContextMenu(const QPoint &pos);
};
