#include "mainwindow.h"
#include "sidebar.h"
#include "tab.h"
#include "tabwidget.h"
#include "utils.h"
#include "webview.h"
#include "window.h"

#include <QEvent>
#include <QSettings>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_centralWidget = new QSplitter;
    setContentsMargins(0, 0, 0, 0);
    setCentralWidget(m_centralWidget);

    m_history = new History(this);
    m_bookmarks = new Bookmarks(this);
    m_cookies = new Cookies(this);
    m_sideBar = new SideBar(this);
    m_sideBar->addItem(QStringLiteral("History"), m_history->historyWidget());
    m_sideBar->addItem(QStringLiteral("Bookmarks"), m_bookmarks->bookmarksWidget());
    m_sideBar->addItem(QStringLiteral("Cookies"), m_cookies->cookiesWidget());
    m_centralWidget->addWidget(m_sideBar);

    setAttribute(Qt::WA_DeleteOnClose);

    QSettings settings;
    if (!settings.value(QStringLiteral("appearance/sidebar"), false).toBool()) {
        m_sideBar->hide();
    }

    restoreGeometry(settings.value(QStringLiteral("mainwindow/geometry")).toByteArray());
    restoreState(settings.value(QStringLiteral("mainwindow/windowstate")).toByteArray());
    if (settings.value(QStringLiteral("mainwindow/geometry")).toByteArray().isEmpty()) {
        resize(800, 600);
    }

    connect(m_history, &History::newTabRequested, [this] (const QUrl &url) {
        Window *window = dynamic_cast<Window *>(m_centralWidget->widget(1));
        if (!window) {
            std::cerr << "[!] unable to cast index 1 of central widget to Window, please report it!" << std::endl;
            return ;
        }

        Tab *tab = new Tab;
        tab->webView()->load(url);
        window->tabWidget()->addTab(tab);
    });

    connect(m_bookmarks, &Bookmarks::newTabRequested, [this] (const QUrl &url) {
        Window *window = dynamic_cast<Window *>(m_centralWidget->widget(1));
        if (!window) {
            std::cerr << "[!] unable to cast index 1 of central widget to Window, please report it!" << std::endl;
            return ;
        }

        Tab *tab = new Tab;
        tab->webView()->load(url);
        window->tabWidget()->addTab(tab);
    });
}

Tab *MainWindow::createWindow()
{
    Window *window = new Window;
    m_centralWidget->addWidget(window);

    connect(window, &Window::newWindowRequested, this, &MainWindow::createWindow);
    connect(window, &Window::newMainWindowRequested, this, [this] { emit newMainWindowRequested(); });
    connect(window, &Window::historyItemInserted, m_history, &History::updateHistoryWidget);

    connect(window, &Window::windowWillClose, this, [this, window] {
        window->setParent(nullptr);
        window->deleteLater();

        if (m_centralWidget->count() == 1) {
            close();
        }
    });

    connect(window, &Window::toggleSideBarRequested, this, [this] {
        if (m_sideBar->isVisible()) {
            m_sideBar->hide();
        } else {
            m_sideBar->show();
        }
    });

    return window->tabWidget()->tabAt(0);
}

void MainWindow::show()
{
    QMainWindow::show();

    Utils::removeTitleBar(winId());
#ifdef __APPLE__
    QSettings settings;
    // TODO: make it as default behaviour
    bool useAccentColorForTheme = settings.value(QStringLiteral("appearance/useaccentcolorfortheme"), false).toBool();
    if (useAccentColorForTheme) {
        Utils::setAccentColorAsWindowBackground(winId());
    }
#endif
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit mainWindowWillClose();

    QSettings settings;
    settings.setValue(QStringLiteral("mainwindow/geometry"), saveGeometry());
    settings.setValue(QStringLiteral("mainwindow/windowstate"), saveState());

    QMainWindow::closeEvent(event);
}

bool MainWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::WinIdChange:
    case QEvent::Paint:
        Utils::removeTitleBar(winId());
        break;
    default:
        break;
    }

    return QMainWindow::event(event);
}
