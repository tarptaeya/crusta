#include "browserwindow.h"
#include "common-defs.h"
#include "mainapplication.h"
#include "statusbar.h"
#include "tab.h"
#include "tabwidget.h"
#include "webview.h"

#include <QFileDialog>

BrowserWindow::BrowserWindow(Tab *tab, QWidget *parent)
    : QMainWindow (parent)
{
    m_statusBar = new StatusBar(this);
    m_tabWidget = new TabWidget(this);

    m_tabWidget->addTab(tab);

    setStatusBar(m_statusBar);
    setCentralWidget(m_tabWidget);

    resize(800, 600);

    loadSettings();

    setAttribute(Qt::WA_DeleteOnClose);

    connect(m_tabWidget, &TabWidget::currentChanged, this, [this](int index) {
        if (appManager->isPrivate()) {
            setWindowTitle(QSL("%1 - Crusta Private").arg(m_tabWidget->tabText(index)));
        } else {
            setWindowTitle(QSL("%1 - Crusta").arg(m_tabWidget->tabText(index)));
        }
    });
}

StatusBar *BrowserWindow::statusBar()
{
    return m_statusBar;
}

TabWidget *BrowserWindow::tabWidget()
{
    return m_tabWidget;
}

void BrowserWindow::loadSettings()
{
    if (appManager->settings()->value(QSL("browserWindow/restoreWindowGeometry"), true).toBool()) {
        restoreGeometry(appManager->settings()->value(QSL("browserWindow/geometry")).toByteArray());
        restoreState(appManager->settings()->value(QSL("browserWindow/geometry")).toByteArray());
    }

    if (!appManager->settings()->value(QSL("browserWindow/showStatusBar"), true).toBool()) {
        m_statusBar->hide();
    }
}

void BrowserWindow::closeEvent(QCloseEvent *event)
{
    appManager->settings()->setValue(QSL("browserWindow/geometry"), saveGeometry());
    appManager->settings()->setValue(QSL("browserWindow/state"), saveState());

    appManager->removeWindow(this);

    QMainWindow::closeEvent(event);
}

void BrowserWindow::openFile()
{
    QFileDialog fileDialog;
    const QUrl filePath = fileDialog.getOpenFileUrl();
    m_tabWidget->currentTab()->webView()->load(filePath);
}
