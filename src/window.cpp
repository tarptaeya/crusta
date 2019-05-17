#include "preferences.h"
#include "tabwidget.h"
#include "tab.h"
#include "toolbar.h"
#include "window.h"

#include <QAction>
#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>

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
    QSettings settings;

    QMenu *file = m_menu->addMenu(QStringLiteral("&File"));
    QMenu *edit = m_menu->addMenu(QStringLiteral("&Edit"));
    QMenu *view = m_menu->addMenu(QStringLiteral("&View"));
    QMenu *history = m_menu->addMenu(QStringLiteral("&History"));
    QMenu *session = m_menu->addMenu(QStringLiteral("&Sessions"));
    QMenu *tools = m_menu->addMenu(QStringLiteral("&Tools"));
    m_menu->addSeparator();
    QAction *preferences = m_menu->addAction(QStringLiteral("Preferences"));

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

    QAction *toggleSideBar = view->addAction(QStringLiteral("Side bar"));
    toggleSideBar->setCheckable(true);
    toggleSideBar->setChecked(true);
    if (!settings.value(QStringLiteral("appearance/sidebar"), false).toBool()) {
        toggleSideBar->setChecked(false);
    }

    QAction *showAllHistory = history->addAction(QStringLiteral("Show All History"));
    QAction *clearAllHistory = history->addAction(QStringLiteral("Clear All History"));

    QAction *responsiveMode = tools->addAction(QStringLiteral("Enter Responsive Design Mode"));
    if (m_tabWidget->currentTab()->isInResponsiveMode()) {
        responsiveMode->setText(QStringLiteral("Exit Responsive Design Mode"));
    }
    QAction *resetCrusta = tools->addAction(QStringLiteral("Reset Crusta"));

    newWindow->setShortcut(QKeySequence::New);

    connect(newWindow, &QAction::triggered, this, [this] { emit newMainWindowRequested(); });
    connect(newPrivateWindow, &QAction::triggered, this, [] { QProcess::startDetached(QStringLiteral("%1 --private").arg(qApp->applicationFilePath())); });
    connect(newSplit, &QAction::triggered, this, [this] { emit newWindowRequested(); });

    connect(toggleSideBar, &QAction::triggered, this, [this] {
        QSettings settings;
        settings.setValue(QStringLiteral("appearance/sidebar"), !settings.value(QStringLiteral("appearance/sidebar"), false).toBool());
        emit toggleSideBarRequested();
    });

    connect(responsiveMode, &QAction::triggered, [this] {
        Tab::responsiveControllPane(m_tabWidget->currentTab());
    });
    connect(resetCrusta, &QAction::triggered, this, [this] {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QStringLiteral("Reset Crusta"), QStringLiteral("Are you sure you want to reset Crusta?\nThis operation cannot be undone"));
        if (reply == QMessageBox::No) {
            return ;
        }

        QSettings().clear();
    });

    connect(preferences, &QAction::triggered, [] {
        Preferences *preferences = new Preferences;
        preferences->show();
    });
}
