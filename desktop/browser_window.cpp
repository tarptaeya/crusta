#include "bookmarks.h"
#include "browser.h"
#include "browser_window.h"
#include "browser_window_p.h"
#include "history.h"
#include "tab.h"
#include "webview.h"

#include <QApplication>
#include <QComboBox>
#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWebEngineCookieStore>

void BrowserWindow::setup_menubar()
{
    QMenuBar *menu_bar = new QMenuBar;

    QMenu *file = menu_bar->addMenu(QStringLiteral("File"));
    QMenu *edit = menu_bar->addMenu(QStringLiteral("Edit"));
    QMenu *view = menu_bar->addMenu(QStringLiteral("View"));
    QMenu *history = menu_bar->addMenu(QStringLiteral("History"));
    QMenu *bookmarks = menu_bar->addMenu(QStringLiteral("Bookmarks"));
    QMenu *tools = menu_bar->addMenu(QStringLiteral("Tools"));

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

    QAction *open_new_private_window = file->addAction(QStringLiteral("New Private Window"));
    connect(open_new_private_window, &QAction::triggered, [] {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList() << QStringLiteral("--private"));
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

    QAction *undo = edit->addAction(QStringLiteral("Undo"));
    undo->setShortcut(QKeySequence::Undo);
    connect(undo, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->triggerPageAction(QWebEnginePage::Undo);
    });

    QAction *redo = edit->addAction(QStringLiteral("Redo"));
    redo->setShortcut(QKeySequence::Undo);
    connect(redo, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->triggerPageAction(QWebEnginePage::Redo);
    });

    QAction *cut = edit->addAction(QStringLiteral("Cut"));
    cut->setShortcut(QKeySequence::Cut);
    connect(cut, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->triggerPageAction(QWebEnginePage::Cut);
    });

    QAction *copy = edit->addAction(QStringLiteral("Copy"));
    copy->setShortcut(QKeySequence::Copy);
    connect(copy, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->triggerPageAction(QWebEnginePage::Copy);
    });

    QAction *paste = edit->addAction(QStringLiteral("Paste"));
    paste->setShortcut(QKeySequence::Paste);
    connect(paste, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->triggerPageAction(QWebEnginePage::Paste);
    });

    QAction *paste_and_match_style = edit->addAction(QStringLiteral("Paste And Match Style"));
    connect(paste_and_match_style, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->triggerPageAction(QWebEnginePage::PasteAndMatchStyle);
    });

    QAction *select_all = edit->addAction(QStringLiteral("Select All"));
    select_all->setShortcut(QKeySequence::SelectAll);
    connect(select_all, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->triggerPageAction(QWebEnginePage::SelectAll);
    });

    edit->addSeparator();

    QAction *find = edit->addAction(QStringLiteral("Find In Page"));
    find->setShortcut(QKeySequence::Find);
    connect(find, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;

        QDialog *fd = new QDialog(tab->webview());
        fd->setAttribute(Qt::WA_DeleteOnClose);
        fd->setModal(true);
        fd->setWindowTitle(QStringLiteral("Find In Page"));
        QVBoxLayout *vbox = new QVBoxLayout;

        QLineEdit *input = new QLineEdit;
        input->setPlaceholderText(QStringLiteral("Find Text"));
        connect(input, &QLineEdit::textChanged, [tab](const QString &text) {
            tab->webview()->findText(text);
        });

        QToolBar *toolbar = new QToolBar;
        QAction *prev = toolbar->addAction(QStringLiteral("Previous"));
        connect(prev, &QAction::triggered, [tab, input] {
            tab->webview()->findText(input->text(), QWebEnginePage::FindBackward);
        });
        QAction *next = toolbar->addAction(QStringLiteral("Next"));
        connect(next, &QAction::triggered, [tab, input] {
            tab->webview()->findText(input->text());
        });

        QWidget *spacer = new QWidget;
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        toolbar->addWidget(spacer);

        QAction *done = toolbar->addAction(QStringLiteral("Done"));
        connect(done, &QAction::triggered, [fd, tab] {
            fd->close();
            tab->webview()->findText(QStringLiteral(""));
        });

        vbox->addWidget(input);
        vbox->addWidget(toolbar);
        fd->setLayout(vbox);
        fd->open();

        if (tab->webview()->hasSelection())
            input->setText(tab->webview()->selectedText());
    });

    QAction *reload = view->addAction(QStringLiteral("Reload Page"));
    reload->setShortcut(QKeySequence::Refresh);
    connect(reload, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->reload();
    });

    view->addSeparator();

    QAction *zoom_in = view->addAction(QStringLiteral("Zoom In"));
    zoom_in->setShortcut(QKeySequence::ZoomIn);
    connect(zoom_in, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->setZoomFactor(0.25 + tab->webview()->zoomFactor());
    });

    QAction *zoom_out = view->addAction(QStringLiteral("Zoom out"));
    zoom_out->setShortcut(QKeySequence::ZoomOut);
    connect(zoom_out, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->setZoomFactor(-0.25 + tab->webview()->zoomFactor());
    });

    QAction *reset_zoom = view->addAction(QStringLiteral("Actual Size"));
    reset_zoom->setShortcut(Qt::CTRL + Qt::Key_0);
    connect(reset_zoom, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->setZoomFactor(1.0);
    });

    view->addSeparator();

    QAction *source = view->addAction(QStringLiteral("View Source"));
    connect(source, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;
        tab->webview()->triggerPageAction(QWebEnginePage::ViewSource);
    });

    view->addSeparator();

    QAction *show_all_history = history->addAction(QStringLiteral("Show All History"));
    connect(show_all_history, &QAction::triggered, [this] {
        ManagerTab *manager = new ManagerTab;
        add_existing_tab(manager);
        manager->open_history();
    });

    history->addSeparator();

    QAction *clear_history = history->addAction(QStringLiteral("Clear History..."));
    connect(clear_history, &QAction::triggered, [this] {
        QDialog *hd = new QDialog(this);
        hd->setAttribute(Qt::WA_DeleteOnClose);
        hd->setModal(true);

        QVBoxLayout *vbox = new QVBoxLayout;
        hd->setLayout(vbox);

        vbox->addWidget(new QLabel(QStringLiteral("<b>Clear History</b>")));

        QComboBox *date = new QComboBox;
        date->addItems(QStringList()
                       << QStringLiteral("The Last Hour")
                       << QStringLiteral("Last 24 Hours")
                       << QStringLiteral("All History"));
        vbox->addWidget(date);

        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->addWidget(new QWidget);
        vbox->addLayout(hbox);

        QPushButton *ok = new QPushButton(QStringLiteral("Clear History"));
        hbox->addWidget(ok);
        connect(ok, &QPushButton::clicked, [hd, date] {
            QDateTime time = QDateTime::currentDateTime();
            switch (date->currentIndex()) {
            case 0:
                time = time.addSecs(-60 * 60);
                break;
            case 1:
                time = time.addDays(-1);
                break;
            case 2:
                browser->history_model()->remove_all();
                hd->close();
                return;
            }

            browser->history_model()->remove_entries_by_date(time);
            hd->close();
        });

        QPushButton *cancel = new QPushButton(QStringLiteral("Cancel"));
        hbox->addWidget(cancel);
        connect(cancel, &QPushButton::clicked, hd, &QDialog::close);

        hd->open();
    });

    QAction *clear_all_history = history->addAction(QStringLiteral("Clear All History"));
    connect(clear_all_history, &QAction::triggered, [this] {
        if (QMessageBox::Yes != QMessageBox::question(this, QStringLiteral("Clear all history"), QStringLiteral("Are you sure to clear all history?")))
            return ;

        browser->history_model()->remove_all();
    });

    QAction *bookmark_this_tab = bookmarks->addAction(QStringLiteral("Boomark This Tab"));
    connect(bookmark_this_tab, &QAction::triggered, [this] {
        WebTab *tab = dynamic_cast<WebTab *>(m_central_widget->current_tab());
        if (!tab)
            return ;

        tab->bookmark();
    });

    QAction *bookmark_all_tabs = bookmarks->addAction(QStringLiteral("Bookmark All Tabs"));
    connect(bookmark_all_tabs, &QAction::triggered, [this] {
        const QList<Tab *> tabs = this->tabs();
        for (int i = 0; i < tabs.count(); i++) {
            WebTab *tab = dynamic_cast<WebTab *>(tabs.at(i));
            if (!tab)
                continue;

            BookmarkTreeNode *node = new BookmarkTreeNode(BookmarkTreeNode::Address);
            node->title = tab->webview()->title();
            node->address = tab->webview()->url().toString();
            browser->bookmark_model()->add_bookmark(nullptr, node);
        }
    });

    bookmarks->addSeparator();

    QAction *show_all_bookmarks = bookmarks->addAction(QStringLiteral("Show All Bookmarks"));
    connect(show_all_bookmarks, &QAction::triggered, [this] {
        ManagerTab *manager = new ManagerTab;
        add_existing_tab(manager);
        manager->open_bookmarks();
    });

    QAction *about_crusta = tools->addAction(QStringLiteral("About Crusta"));
    connect(about_crusta, &QAction::triggered, [] {
        QDialog *ad = new QDialog;
        ad->setAttribute(Qt::WA_DeleteOnClose);
        ad->setSizeGripEnabled(false);
        QVBoxLayout *vbox = new QVBoxLayout;
        ad->setLayout(vbox);

        QFont small_font;
        small_font.setPointSize(10);

        QLabel *name = new QLabel(QStringLiteral("<b>Crusta</b>"));
        name->setAlignment(Qt::AlignCenter);
        vbox->addWidget(name);

        QLabel *version = new QLabel(QStringLiteral("Version 2.0.0-alpha"));
        version->setFont(small_font);
        version->setAlignment(Qt::AlignCenter);
        vbox->addWidget(version);

        QLabel *copyright = new QLabel(QStringLiteral("Copyright © 2017-2020 Anmol Gautam.\nGNU GPLv3 License."));
        copyright->setFont(small_font);
        copyright->setAlignment(Qt::AlignCenter);
        vbox->addWidget(copyright);

        ad->setFixedSize(ad->sizeHint());
        ad->open();
    });

    QAction *delete_session_cookies = tools->addAction(QStringLiteral("Delete Session Cookies"));
    connect(delete_session_cookies, &QAction::triggered, [] {
        browser->web_profile()->cookieStore()->deleteSessionCookies();
    });

    QAction *delete_all_cookies = tools->addAction(QStringLiteral("Delete All Cookies"));
    connect(delete_all_cookies, &QAction::triggered, [] {
        browser->web_profile()->cookieStore()->deleteAllCookies();
    });

    tools->addSeparator();

    QAction *clear_http_cache = tools->addAction(QStringLiteral("Clear HTTP Cache"));
    connect(clear_http_cache, &QAction::triggered, [] {
        browser->web_profile()->clearHttpCache();
    });

    QAction *preferences = tools->addAction(QStringLiteral("Preferences"));
    connect(preferences, &QAction::triggered, [this] {
        ManagerTab *manager = new ManagerTab;
        add_existing_tab(manager);
        manager->open_settings();
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

    QToolBar *toolbar = new QToolBar;
    m_tabbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    if (browser->is_private()) {
        QLabel *label = new QLabel;
        label->setPixmap(QIcon::fromTheme(QStringLiteral("view-private")).pixmap(16, 16));
        label->setFixedWidth(24);
        label->setAlignment(Qt::AlignCenter);
        toolbar->addWidget(label);
    }
    toolbar->addWidget(m_tabbar);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);
    setLayout(vbox);

    vbox->addWidget(toolbar);
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
