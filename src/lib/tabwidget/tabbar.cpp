#include "common-defs.h"
#include "mainapplication.h"
#include "tab.h"
#include "tabbar.h"
#include "tabwidget.h"
#include "webpage.h"
#include "webview.h"

#include <QAction>
#include <QApplication>
#include <QMenu>

#define MAX_TAB_WIDTH 225
#define NEW_TAB_BUTTON_WIDTH 24
#define NEW_TAB_BUTTON_MARGIN 2

TabBar::TabBar(QWidget *parent)
    : QTabBar (parent)
{
    m_newTabButton = new QToolButton(this);
    m_newTabButton->setAutoRaise(true);
    m_newTabButton->setIcon(QIcon(QSL(":/icons/arrows_plus.svg")));
    m_newTabButton->setFixedWidth(NEW_TAB_BUTTON_WIDTH);
    m_newTabButton->setToolTip(QSL("Add New Tab"));
    m_newTabButton->setShortcut(QKeySequence::AddTab);

    m_tabWidget = static_cast<TabWidget *>(this->parent());

    // Settings which cannot be changed
    setTabsClosable(true);
    setMovable(true);
    setDocumentMode(true);
    setElideMode(Qt::ElideRight);
    setMouseTracking(true);

    loadSettings();

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_newTabButton, &QToolButton::clicked, [this] {
        if (!m_tabWidget) {
            return;
        }

        m_tabWidget->addTab();
        m_tabWidget->setCurrentIndex(m_tabWidget->count() - 1);
    });

    connect(this, &TabBar::tabCloseRequested, this, [this] {
        if (!m_isInClosingState) {
            m_tabCountForClosingState = count();
        }

        m_isInClosingState = true;
    });

    connect(this, &TabBar::tabBarDoubleClicked, this, [this] (int index) {
        if (index > -1) {
            return;
        }

        m_newTabButton->click();
    });

    connect(this, &TabBar::customContextMenuRequested, this, &TabBar::showContextMenu);
}

void TabBar::loadSettings()
{
    setUsesScrollButtons(appManager->settings()->value(QSL("tabBar/setUsesScrollButtons"), true).toBool());
}

int TabBar::getTabWidth(int index) const
{
    int tabBarWidth = width() - NEW_TAB_BUTTON_WIDTH - 2 * NEW_TAB_BUTTON_MARGIN;
    int numOfTabs = qMax(count(), 1);
    if (m_isInClosingState) {
        numOfTabs = m_tabCountForClosingState;
    }

    int baseWidth = tabBarWidth / numOfTabs;
    int residualWidth = qMax(0, tabBarWidth % numOfTabs - index);
    baseWidth += qMin(1, residualWidth);
    baseWidth = qMin(baseWidth, MAX_TAB_WIDTH);

    return baseWidth;
}

void TabBar::showContextMenu(const QPoint &pos)
{
    int index = tabAt(pos);
    if (index == -1) {
        return;
    }

    TabWidget *tabWidget = static_cast<TabWidget *>(this->parent());
    if (!tabWidget) {
        return;
    }
    Tab *tab = tabWidget->tabAt(index);

    QMenu menu;
    QAction *duplicate = menu.addAction(QSL("Duplicate Tab"));
    QAction *mute = menu.addAction(QSL("Mute Tab"));
    menu.addSeparator();
    QAction *closeLeft = menu.addAction(QSL("Close All Tabs to Left"));
    QAction *closeRight = menu.addAction(QSL("Close All Tabs to Right"));
    QAction *closeOther = menu.addAction(QSL("Close Other Tabs"));

    if (tab->webView()->page()->isAudioMuted()) {
        mute->setText(QSL("Unmute Tab"));
    }

    connect(duplicate, &QAction::triggered, this, [tab, tabWidget] {
        Tab *newTab = new Tab(tab->webView()->url().toString());
        tabWidget->addTab(newTab);
    });
    connect(mute, &QAction::triggered, this, [tab] {
        tab->webView()->page()->setAudioMuted(!tab->webView()->page()->isAudioMuted());
    });
    connect(closeLeft, &QAction::triggered, this, [index, tabWidget] {
        for (int i = 0; i < index; i++) {
            tabWidget->closeTab(0);
        }
    });
    connect(closeRight, &QAction::triggered, this, [index, tabWidget] {
        int count = tabWidget->count();
        for (int i = index + 1; i < count; i++) {
            tabWidget->closeTab(index + 1);
        }
    });
    connect(closeOther, &QAction::triggered, this, [closeLeft, closeRight] {
        closeRight->trigger();
        closeLeft->trigger();
    });

    menu.exec(mapToGlobal(pos));
}

QSize TabBar::tabSizeHint(int index) const
{
    QSize size = QTabBar::tabSizeHint(index);
    int width = getTabWidth(index);
    size.setWidth(width);
    return size;
}

void TabBar::paintEvent(QPaintEvent *event)
{
    QTabBar::paintEvent(event);

    int xCoord = NEW_TAB_BUTTON_MARGIN;
    for (int i = 0; i < count(); i++) {
        xCoord += getTabWidth(i);
    }
    if (qApp->layoutDirection() == Qt::RightToLeft) {
        xCoord = width() - xCoord - NEW_TAB_BUTTON_WIDTH;
    }

    int yCoord = (height() - m_newTabButton->height()) / 2;

    m_newTabButton->move(xCoord, yCoord);
}

void TabBar::mouseMoveEvent(QMouseEvent *event)
{
    QTabBar::mouseMoveEvent(event);
    if (m_isInClosingState) {
        // workaround for resizing tabs
        m_tabCountForClosingState = count();
        m_tabWidget->addTab(QSL(""));
        tabCloseRequested(count() - 1);
    }
    m_isInClosingState = false;
}
