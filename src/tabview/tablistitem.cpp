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
#include "tablistitem.h"
#include "tab.h"
#include "tabwidget.h"
#include "../webview/webview.h"
#include "../utils/dimensions.h"
#include <QIcon>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QPalette>

TabListItem::TabListItem(QWidget *parent)
    : QWidget(parent)
{
    m_hBoxLayout = new QHBoxLayout;
    m_hBoxLayout->setContentsMargins(5, 0, 5, 0);
    setLayout(m_hBoxLayout);

    m_favicon = new QLabel(this);
    m_favicon->setFixedSize(20, 20); // arbitrary number greater than 16
    m_title = new QLabel(this);
    m_closeTabButton = new QClickableLabel(this);
    m_closeTabButton->setFixedSize(20, 20);
    m_closeTabButton->setText("X");

    m_hBoxLayout->addWidget(m_favicon);
    m_hBoxLayout->addWidget(m_title);
    m_hBoxLayout->addWidget(m_closeTabButton);
    setFixedHeight(Dimensions::tabListItemHeight());

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &TabListItem::customContextMenuRequested, this, &TabListItem::showContextMenu);
}

void TabListItem::setVirtualTab(Tab *tab)
{
    m_tab = tab;
    tab->setTabListItem(this);
    connect(m_tab->webview(), &WebView::iconChanged, this, &TabListItem::setFavicon);
    connect(m_tab->webview(), &WebView::titleChanged, this, &TabListItem::setTitle);
    connect(m_closeTabButton, &QClickableLabel::clicked, this, [this]{
        if (!m_tabWidget) {
            return;
        }
        m_tabWidget->closeTab(m_tab);
    });
}

void TabListItem::setVirtualTabWidget(TabWidget *tabWidget)
{
    m_tabWidget = tabWidget;
}

void TabListItem::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        m_tabWidget->currentTabChanged(m_tab);
    }
}

void TabListItem::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (m_tab) {
        setTitle(m_tab->webview()->title());
    }
}

void TabListItem::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    if (m_state == Current) {
        return;
    }

    m_state = Hovered;
    update();
}

void TabListItem::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    if (m_state == Current) {
        return;
    }

    m_state = Normal;
    update();
}

void TabListItem::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (m_state == Current) {
        QPainter painter(this);
        painter.fillRect(0, 0, width(), height(), qApp->palette().highlight().color());
    } else if (m_state == Hovered) {
        QPainter painter(this);
        painter.fillRect(0, 0, width(), height(), qApp->palette().mid().color());
    }
}

void TabListItem::setCurrent(bool current)
{
    if (current) {
        m_state = Current;
    } else {
        m_state = Normal;
    }
    update();
}

void TabListItem::setFavicon(const QIcon &favicon)
{
    m_favicon->setPixmap(favicon.pixmap(16, 16));
}

void TabListItem::setTitle(const QString &title)
{
    const QString elidedTitle = m_title->fontMetrics().elidedText(title, Qt::ElideRight, m_title->width() - 10); // 10 is arbitrary
    m_title->setText(elidedTitle);
}

void TabListItem::showContextMenu(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    QAction *copyUrlAction = menu->addAction(tr("Copy URL"));
    connect(copyUrlAction, &QAction::triggered, this, [this]{
        qApp->clipboard()->setText(m_tab->webview()->url().toString());
    });
    QAction *muteUnmuteAction;
    if (m_tab->webview()->page()->isAudioMuted()) {
        muteUnmuteAction = menu->addAction(tr("Unmute Tab"));
    } else {
        muteUnmuteAction = menu->addAction(tr("Mute Tab"));
    }
    connect(muteUnmuteAction, &QAction::triggered, this, [this]{
        m_tab->webview()->page()->setAudioMuted(!m_tab->webview()->page()->isAudioMuted());
    });
    QAction *closeAction = menu->addAction(tr("Close Tab"));
    connect(closeAction, &QAction::triggered, this, [this]{
        m_tabWidget->closeTab(m_tab);
    });
    QAction * closeAfterAction = menu->addAction(tr("Close all After"));
    connect(closeAfterAction, &QAction::triggered, this, [this]{
        m_tabWidget->closeTabsAfter(m_tab);
    });
    QAction * closeBeforeAction = menu->addAction(tr("Close all Before"));
    connect(closeBeforeAction, &QAction::triggered, this, [this]{
        m_tabWidget->closeTabsBefore(m_tab);
    });
    menu->exec(mapToGlobal(pos));
}
