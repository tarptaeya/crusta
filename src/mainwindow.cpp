#include "mainwindow.h"
#include "sidebar.h"
#include "utils.h"
#include "window.h"

#include <QDebug>
#include <QEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_centralWidget = new QSplitter;
    setContentsMargins(0, 0, 0, 0);
    setCentralWidget(m_centralWidget);

    m_history = new History;
    m_sideBar = new SideBar;
    m_sideBar->addItem(QStringLiteral("History"), m_history->historyWidget());
    m_centralWidget->addWidget(m_sideBar);

    createWindow();

    setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::createWindow()
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
}

void MainWindow::show()
{
    QMainWindow::show();

    Utils::removeTitleBar(winId());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit mainWindowWillClose();

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
