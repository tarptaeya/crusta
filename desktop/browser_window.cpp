#include "browser.h"
#include "browser_window.h"
#include "browser_window_p.h"
#include "tab.h"
#include "webview.h"

#include <QApplication>
#include <QFileDialog>
#include <QMenuBar>
#include <QVBoxLayout>

void BrowserWindow::setup_menubar()
{
    QMenuBar *menu_bar = new QMenuBar;

    QMenu *file = menu_bar->addMenu(QStringLiteral("File"));
    QMenu *history = menu_bar->addMenu(QStringLiteral("History"));
    QMenu *bookmarks = menu_bar->addMenu(QStringLiteral("Bookmarks"));

    QAction *open_new_tab = file->addAction(QStringLiteral("New Tab"));
    open_new_tab->setShortcut(QKeySequence::AddTab);
    connect(open_new_tab, &QAction::triggered, this, &BrowserWindow::add_new_tab);

    QAction *open_new_window = file->addAction(QStringLiteral("New Window"));
    open_new_window->setShortcut(QKeySequence::New);
    connect(open_new_window, &QAction::triggered, [] {
        BrowserWindow *window = browser->create_browser_window();
        WebTab *tab = (WebTab *) window->tabs().at(0);
        tab->webview()->home();
    });

    QAction *open_file = file->addAction(QStringLiteral("Open File..."));
    open_file->setShortcut(QKeySequence::Open);
    connect(open_file, &QAction::triggered, [this] {
        QFileDialog fd;
        const QUrl address = fd.getOpenFileUrl(this);
        if (address.isEmpty())
            return ;

        WebTab *tab = new WebTab;
        add_existing_tab(tab);
        tab->webview()->load(address);
    });

    QAction *open_location = file->addAction(QStringLiteral("Open Location..."));
    connect(open_location, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;

        tab->address_bar()->setFocus();
        tab->address_bar()->selectAll();
    });

    file->addSeparator();

    QAction *close_tab = file->addAction(QStringLiteral("Close Tab"));
    close_tab->setShortcut(QKeySequence::Close);
    connect(close_tab, &QAction::triggered, [this] {
        m_central_widget->remove_tab(m_central_widget->current_index());
    });

    QAction *close_window = file->addAction(QStringLiteral("Close Window"));
    close_window->setShortcut(Qt::SHIFT + Qt::CTRL + Qt::Key_W);
    connect(close_window, &QAction::triggered, this, &BrowserWindow::close);

    QAction *close_all_windows = file->addAction(QStringLiteral("Close All Windows"));
    connect(close_all_windows, &QAction::triggered, qApp, &QApplication::closeAllWindows);

    file->addSeparator();

    QAction *export_to_pdf = file->addAction(QStringLiteral("Export to PDF"));
    export_to_pdf->setShortcut(QKeySequence::Print);
    connect(export_to_pdf, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        QFileDialog fd;
        QString address = fd.getSaveFileUrl(this).toLocalFile();
        if (address.isEmpty())
            return;
        if (!address.endsWith(QStringLiteral(".pdf")))
            address.append(QStringLiteral(".pdf"));
        tab->webview()->page()->printToPdf(address);
    });

    QAction *show_all_history = history->addAction(QStringLiteral("Show All History"));
    connect(show_all_history, &QAction::triggered, [this] {
        ManagerTab *manager = new ManagerTab;
        add_existing_tab(manager);
        manager->open_history();
    });

    QAction *show_all_bookmarks = bookmarks->addAction(QStringLiteral("Show All Bookmarks"));
    connect(show_all_bookmarks, &QAction::triggered, [this] {
        ManagerTab *manager = new ManagerTab;
        add_existing_tab(manager);
        manager->open_bookmarks();
    });

    setMenuBar(menu_bar);
}

BrowserWindow::BrowserWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);

    m_central_widget = new CentralWidget;
    setCentralWidget(m_central_widget);

    setup_menubar();
    setUnifiedTitleAndToolBarOnMac(true);
}

Tab *BrowserWindow::add_new_tab()
{
    return m_central_widget->add_new_tab();
}

Tab *BrowserWindow::add_existing_tab(Tab *tab)
{
    return m_central_widget->add_existing_tab(tab);
}

QList<Tab *> BrowserWindow::tabs() const
{
    return m_central_widget->tabs();
}

void CentralWidget::setup_tabbar()
{
    connect(m_tabbar, &NormalTabbar::currentChanged, m_stacked_widget, &QStackedWidget::setCurrentIndex);
    connect(m_tabbar, &NormalTabbar::tabMoved, [this](int from, int to) {
        m_stacked_widget->insertWidget(to, m_stacked_widget->widget(from));
    });
    connect(m_tabbar, &NormalTabbar::new_tab_requested, this, &CentralWidget::add_new_tab);
    connect(m_tabbar, &NormalTabbar::tabCloseRequested, this, &CentralWidget::remove_tab);
}

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
{
    m_stacked_widget = new QStackedWidget;
    m_tabbar = new NormalTabbar;

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);
    setLayout(vbox);

    vbox->addWidget(m_tabbar);
    vbox->addWidget(m_stacked_widget);

    setup_tabbar();

    add_new_tab();
}

Tab *CentralWidget::add_new_tab()
{
    WebTab *tab = new WebTab;
    return add_existing_tab(tab);
}

Tab *CentralWidget::add_existing_tab(Tab *tab)
{
    int index = m_tabbar->addTab(QStringLiteral("New Tab"));
    m_stacked_widget->addWidget(tab);
    m_tabbar->setCurrentIndex(index);

    connect(tab, &Tab::title_changed, [this, tab] (const QString &title) {
        int index = m_stacked_widget->indexOf(tab);
        m_tabbar->setTabText(index, title);
    });

    connect(tab, &Tab::icon_changed, [this, tab] (const QIcon &icon) {
        int index = m_stacked_widget->indexOf(tab);
        m_tabbar->setTabIcon(index, icon);
    });

    return tab;
}

void CentralWidget::remove_tab(int index)
{
    Tab *tab = dynamic_cast<Tab *>(m_stacked_widget->widget(index));
    if (!tab) {
        return;
    }

    m_tabbar->removeTab(index);
    m_stacked_widget->removeWidget(tab);
    tab->deleteLater();
}

int CentralWidget::current_index() const
{
    return m_stacked_widget->currentIndex();
}

Tab *CentralWidget::current_tab() const
{
    return dynamic_cast<Tab *>(m_stacked_widget->currentWidget());
}

QList<Tab *> CentralWidget::tabs() const
{
    QList<Tab *> list;
    for (int i = 0; i < m_stacked_widget->count(); i++) {
        Tab *tab = dynamic_cast<Tab *>(m_stacked_widget->widget(i));
        if (!tab) continue;
        list.append(tab);
    }
    return list;
}

NormalTabbar::NormalTabbar(QWidget *parent)
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

QSize NormalTabbar::tabSizeHint(int index) const
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
