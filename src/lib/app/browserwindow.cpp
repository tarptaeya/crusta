#include "browserwindow.h"
#include "common-defs.h"
#include "mainapplication.h"
#include "statusbar.h"
#include "tabwidget.h"

BrowserWindow::BrowserWindow(const QString &startPageUrl, QWidget *parent)
    : QMainWindow (parent)
{
    m_statusBar = new StatusBar(this);
    m_tabWidget = new TabWidget(this);

    m_tabWidget->addTab(startPageUrl);

    setStatusBar(m_statusBar);
    setCentralWidget(m_tabWidget);

    resize(800, 600);

    loadSettings();

    setAttribute(Qt::WA_DeleteOnClose);

    connect(m_tabWidget, &TabWidget::currentChanged, this, [this](int index) { setWindowTitle(m_tabWidget->tabText(index)); });
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
    restoreGeometry(appManager->settings()->value(QSL("browserWindow/geometry")).toByteArray());
    restoreState(appManager->settings()->value(QSL("browserWindow/geometry")).toByteArray());
    if (!appManager->settings()->value(QSL("browserWindow/showStatusBar"), true).toBool()) {
        m_statusBar->hide();
    }
}

void BrowserWindow::closeEvent(QCloseEvent *event)
{
    appManager->settings()->setValue(QSL("browserWindow/geometry"), saveGeometry());
    appManager->settings()->setValue(QSL("browserWindow/state"), saveState());

    QMainWindow::closeEvent(event);
}
