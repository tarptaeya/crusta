#include "mainwindow.h"
#include "window.h"

#include <QDebug>


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

    connect(window, &Window::windowWillClose, this, [this, window] {
        window->setParent(nullptr);
        window->deleteLater();

        if (m_centralWidget->count() == 0) {
            close();
        }
    });
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit mainWindowWillClose();

    QMainWindow::closeEvent(event);
}
