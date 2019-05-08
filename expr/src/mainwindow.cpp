#include "mainwindow.h"
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

    createWindow();

    setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::createWindow()
{
    Window *window = new Window;
    m_centralWidget->addWidget(window);

    connect(window, &Window::newWindowRequested, this, &MainWindow::createWindow);
    connect(window, &Window::newMainWindowRequested, this, [this] { emit newMainWindowRequested(); });

    connect(window, &Window::windowWillClose, this, [this, window] {
        window->setParent(nullptr);
        window->deleteLater();

        if (m_centralWidget->count() == 0) {
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
    case QEvent::Resize:
        Utils::removeTitleBar(winId());
        break;
    default:
        break;
    }

    return QMainWindow::event(event);
}
