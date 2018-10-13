/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <tarptaeya@gmail.com>
*
* THIS FILE IS A PART OF CRUSTA
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#pragma once

#include "qclickablelabel.h"
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
