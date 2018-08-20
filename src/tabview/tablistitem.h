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
public:
    explicit TabListItem(QWidget *parent = nullptr);
    void setVirtualTab(Tab *tab);
    void setVirtualTabWidget(TabWidget *tabWidget);
    void mousePressEvent(QMouseEvent *event);
private:
    QHBoxLayout *m_hBoxLayout = nullptr;
    Tab *m_tab = nullptr;
    TabWidget *m_tabWidget = nullptr;
    QLabel *m_favicon = nullptr;
    QLabel *m_title = nullptr;
    QClickableLabel *m_closeTabButton = nullptr;

    void setFavicon(const QIcon &favicon);
    void setTitle(const QString &title);
};
