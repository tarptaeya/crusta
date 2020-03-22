#include "browser_window.h"
#include "browser_window_p.h"
#include "tab.h"

#include <QApplication>
#include <QMenuBar>
#include <QVBoxLayout>

void BrowserWindow::setup_menubar()
{
    QMenuBar *menu_bar = new QMenuBar;
    QMenu *edit_menu = menu_bar->addMenu(QStringLiteral("Edit"));
    QAction *preferences = edit_menu->addAction(QStringLiteral("Preferences"));
    setMenuBar(menu_bar);
}

BrowserWindow::BrowserWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    m_central_widget = new CentralWidget;
    setCentralWidget(m_central_widget);

    setup_menubar();
}

void CentralWidget::setup_tabbar()
{
    connect(m_tabbar, &BrowserTabbar::currentChanged, m_stacked_widget, &QStackedWidget::setCurrentIndex);
    connect(m_tabbar, &BrowserTabbar::new_tab_requested, this, &CentralWidget::add_new_tab);
}

void CentralWidget::setup_toolbar()
{
}

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{
    m_stacked_widget = new QStackedWidget;
    m_tabbar = new BrowserTabbar;
    m_toolbar = new QToolBar;

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);
    setLayout(vbox);

    vbox->addWidget(m_tabbar);
    vbox->addWidget(m_toolbar);
    vbox->addWidget(m_stacked_widget);

    setup_tabbar();
    setup_toolbar();

    add_new_tab();
}

Tab *CentralWidget::add_new_tab()
{
    Tab *tab = new Tab;
    int index = m_tabbar->addTab(QStringLiteral("New Tab"));
    m_tabbar->setCurrentIndex(index);
    m_stacked_widget->addWidget(tab);
    return tab;
}

BrowserTabbar::BrowserTabbar(QWidget *parent)
    : QTabBar(parent)
{
    setElideMode(Qt::ElideRight);
    setExpanding(false);
    setMovable(true);
    setTabsClosable(true);
    setUsesScrollButtons(true);

    m_add_tab_button = new QToolButton(this);
    m_add_tab_button->setFixedSize(24, 24);
    m_add_tab_button->setAutoRaise(true);
    m_add_tab_button->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));

    connect(this, &QTabBar::currentChanged, [this] (int index) {
        const int tab_width = tabSizeHint(index).width();
        for (int i = 0; i < count(); i++) {
            QWidget *button = tabButton(i, QTabBar::RightSide);
            if (button) {
                button->setVisible(tab_width > 100);
            }
        }

        QWidget *button = tabButton(index, QTabBar::RightSide);
        if (button) {
            button->setVisible(true);
        }
    });

    connect(m_add_tab_button, &QToolButton::clicked, [this] { emit new_tab_requested(); });
}

QSize BrowserTabbar::tabSizeHint(int index) const
{
    QSize size = QTabBar::tabSizeHint(index);

    const int MAX_TAB_WIDTH = 300;
    const int PADDING = 4;
    const int ADD_BUTTON_WIDTH = PADDING + m_add_tab_button->width() + PADDING;

    int tabbar_width = width() - ADD_BUTTON_WIDTH;
    int tab_width = tabbar_width / (count() > 0 ? count() : 1);
    tab_width = qMin(tab_width, MAX_TAB_WIDTH);

    QWidget *button = tabButton(index, QTabBar::RightSide);
    if (button) {
        button->setVisible(tab_width > 100 || index == currentIndex());
    }

    if (index == 0) {
        int x, y;
        if (tab_width < MAX_TAB_WIDTH) {
            x = tabbar_width;
        } else {
            x = tab_width * (count() > 0 ? count() : 1);
        }

        if (qApp->layoutDirection() == Qt::RightToLeft) {
            x = width() - x - m_add_tab_button->width() - PADDING * 2;
        }

        y = (height() - m_add_tab_button->height()) / 2;
        m_add_tab_button->move(x + PADDING, y);
    }

    size.setWidth(tab_width);
    return size;
}
