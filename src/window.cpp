#include "tabwidget.h"
#include "toolbar.h"
#include "window.h"

#include <QAction>

Window::Window(QWidget *parent)
    : QMainWindow (parent)
{
    m_toolBar = new ToolBar;
    m_tabWidget = new TabWidget;
    m_menu = new QMenu;

    QWidget *centralWidget = new QWidget;
    QHBoxLayout *centralWidgetLayout = new QHBoxLayout;

    setupMenu();
    m_toolBar->setMenu(m_menu);
    addToolBar(m_toolBar);
    setUnifiedTitleAndToolBarOnMac(true);

    centralWidgetLayout->setSpacing(0);
    centralWidgetLayout->setContentsMargins(0, 0, 0, 0);
    centralWidget->setLayout(centralWidgetLayout);
    setCentralWidget(centralWidget);
    setContentsMargins(0, 0, 0, 0);

    centralWidgetLayout->addWidget(m_tabWidget);

    setAttribute(Qt::WA_DeleteOnClose);

    m_toolBar->setTabWidget(m_tabWidget);

    connect(m_tabWidget, &TabWidget::windowCloseRequested, this, &Window::close);
    connect(m_tabWidget, &TabWidget::historyItemInserted, this, [this] { emit historyItemInserted(); });
}

QMenu *Window::menu()
{
    return m_menu;
}

TabWidget *Window::tabWidget()
{
    return m_tabWidget;
}

void Window::closeEvent(QCloseEvent *event)
{
    emit windowWillClose();

    QMainWindow::closeEvent(event);
}

void Window::setupMenu()
{
    QMenu *file = m_menu->addMenu(QStringLiteral("&File"));
    QMenu *edit = m_menu->addMenu(QStringLiteral("&Edit"));
    QMenu *view = m_menu->addMenu(QStringLiteral("&View"));
    QMenu *history = m_menu->addMenu(QStringLiteral("&History"));
    QMenu *session = m_menu->addMenu(QStringLiteral("&Sessions"));
    QMenu *tools = m_menu->addMenu(QStringLiteral("&Tools"));

    QAction *newWindow = file->addAction(QStringLiteral("New Window"));
    QAction *newPrivateWindow = file->addAction(QStringLiteral("New Private Window"));
    QAction *newSplit = file->addAction(QStringLiteral("Add Split Window"));
    file->addSeparator();
    QAction *open = file->addAction(QStringLiteral("Open File"));
    QAction *save = file->addAction(QStringLiteral("Save Page"));
    QAction *print = file->addAction(QStringLiteral("Print to PDF"));

    QAction *cut = edit->addAction(QStringLiteral("Cut"));
    QAction *copy = edit->addAction(QStringLiteral("Copy"));
    QAction *paste = edit->addAction(QStringLiteral("Paste"));
    edit->addSeparator();

    QAction *toggleSideBar = view->addAction(QStringLiteral("Toggle sidebar"));

    newWindow->setShortcut(QKeySequence::New);

    connect(newWindow, &QAction::triggered, this, [this] { emit newMainWindowRequested(); });
    connect(newSplit, &QAction::triggered, this, [this] { emit newWindowRequested(); });

    connect(toggleSideBar, &QAction::triggered, this, [this] { emit toggleSideBarRequested(); });
}
