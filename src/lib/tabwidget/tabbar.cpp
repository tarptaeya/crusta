#include "common-defs.h"
#include "mainapplication.h"
#include "tabbar.h"
#include "tabwidget.h"

#include <QApplication>

#define MAX_TAB_WIDTH 225
#define NEW_TAB_BUTTON_WIDTH 24
#define NEW_TAB_BUTTON_MARGIN 2

TabBar::TabBar(QWidget *parent)
    : QTabBar (parent)
{
    m_newTabButton = new QToolButton(this);
    m_newTabButton->setAutoRaise(true);
    m_newTabButton->setIcon(QIcon::fromTheme(QSL("list-add")));
    m_newTabButton->setFixedWidth(NEW_TAB_BUTTON_WIDTH);
    m_newTabButton->setToolTip(QSL("Add New Tab"));
    m_newTabButton->setShortcut(QKeySequence::AddTab);

    // Settings which cannot be changed
    setTabsClosable(true);
    setMovable(true);
    setDocumentMode(true);
    setElideMode(Qt::ElideRight);
    setMouseTracking(true);

    loadSettings();

    connect(m_newTabButton, &QToolButton::clicked, [this] {
        TabWidget *tabWidget = static_cast<TabWidget *>(this->parent());
        if (!tabWidget) {
            return;
        }

        tabWidget->addTab();
        tabWidget->setCurrentIndex(tabWidget->count() - 1);
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
}

void TabBar::loadSettings()
{
    setUsesScrollButtons(appManager->settings()->value(QSL("tabBar/setUsesScrollButtons"), true).toBool());
}

int TabBar::getTabWidth(int index) const
{
    int tabBarWidth = width() - NEW_TAB_BUTTON_WIDTH - 2 * NEW_TAB_BUTTON_MARGIN;
    int numOfTabs = count();
    if (m_isInClosingState) {
        numOfTabs = m_tabCountForClosingState;
    }

    int baseWidth = tabBarWidth / numOfTabs;
    int residualWidth = qMax(0, tabBarWidth % numOfTabs - index);
    baseWidth += qMin(1, residualWidth);
    baseWidth = qMin(baseWidth, MAX_TAB_WIDTH);

    return baseWidth;
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
        m_newTabButton->click();
        tabCloseRequested(count() - 1);
    }
    m_isInClosingState = false;
}
