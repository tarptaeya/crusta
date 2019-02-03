#include "common-defs.h"
#include "browserwindow.h"
#include "database.h"
#include "mainapplication.h"
#include "manager.h"
#include "menu.h"
#include "preferences.h"
#include "statusbar.h"
#include "tab.h"
#include "tabwidget.h"
#include "webpage.h"
#include "webview.h"

#include <QAction>
#include <QDir>
#include <QMessageBox>
#include <QPageSetupDialog>
#include <QPrinter>
#include <QProcess>
#include <QStandardPaths>
#include <QWebEngineCookieStore>

QList<HistoryItem> Menu::s_historyItems;

Menu::Menu(QWidget *parent)
    : QMenu (parent)
{
    setUpMenu();

    connect(this, &QMenu::aboutToShow, this, &Menu::setUpMenu);
}

void Menu::setUpMenu()
{
    clear();

    QAction *newTab = new QAction(QSL("New Tab"));
    QAction *newWindow = new QAction(QSL("New Window"));
    QAction *newPrivateWindow = new QAction(QSL("New Private Window"));
    QAction *openFile = new QAction(QSL("Open File"));
    QAction *savePage = new QAction(QSL("Save Page as PDF"));
    QAction *pringPage = new QAction(QSL("Print"));

    QMenu *view = new QMenu(QSL("View"));
    QAction *statusBar = new QAction(QSL("Status Bar"));
    QAction *zoomIn = new QAction(QSL("Zoom In"));
    QAction *zoomOut = new QAction(QSL("Zoom Out"));
    QAction *resetZoom = new QAction(QSL("Reset Zoom"));
    QAction *pageSource = new QAction(QSL("Page Source"));
    QAction *fullScreen = new QAction(QSL("Show Full Screen"));

    statusBar->setCheckable(true);
    if (appManager->windows().length() > 0 && appManager->currentWindow()->statusBar()->isVisible()) {
        statusBar->setChecked(true);
    } else {
        statusBar->setChecked(false);
    }

    if (appManager->windows().length() > 0 && appManager->currentWindow()->isFullScreen()) {
        fullScreen->setText(QSL("Exit Full Screen"));
    }

    view->addAction(statusBar);
    view->addSeparator();
    view->addAction(zoomIn);
    view->addAction(zoomOut);
    view->addAction(resetZoom);
    view->addSeparator();
    view->addAction(pageSource);
    view->addAction(fullScreen);

    QMenu *history = new QMenu(QSL("History"));
    QAction *showAllHistory = new QAction(QSL("Show All History"));
    QAction *clearAllCookies = new QAction(QSL("Clear All Cookies"));
    QAction *clearAllHistory = new QAction(QSL("Clear All History"));
    QMenu *recentlyClosed = new QMenu(QSL("Recently Closed"));

    history->addAction(showAllHistory);
    history->addSeparator();
    history->addAction(clearAllCookies);
    history->addAction(clearAllHistory);
    history->addSeparator();
    history->addMenu(recentlyClosed);

    for (const HistoryItem &item : s_historyItems) {
        QAction *action = new QAction(item.icon, item.title);
        recentlyClosed->addAction(action);

        connect(action, &QAction::triggered, this, [item] { appManager->currentWindow()->tabWidget()->addTab(item.url); });
    }

    QMenu *bookmarks = new QMenu(QSL("Bookmarks"));
    QAction *showAllBookmarks = new QAction(QSL("Show All Bookmarks"));
    bookmarks->addAction(showAllBookmarks);

    QMenu *downloads = new QMenu(QSL("Downloads"));
    QAction *showAllDownloads = new QAction(QSL("Show All Downloads"));
    downloads->addAction(showAllDownloads);

    QAction *preferences = new QAction(QSL("Preferences"));
    QAction *about = new QAction(QSL("About Crusta"));
    QAction *help = new QAction(QSL("Help"));

    addAction(newTab);
    addAction(newWindow);
    addAction(newPrivateWindow);
    addSeparator();
    addAction(openFile);
    addAction(savePage);
    addAction(pringPage);
    addSeparator();
    addMenu(view);
    addMenu(history);
    addMenu(bookmarks);
    addMenu(downloads);
    addSeparator();
    addAction(preferences);
    addAction(about);
    addAction(help);

    openFile->setShortcut(QKeySequence::Open);
    zoomIn->setShortcut(QKeySequence::ZoomIn);
    zoomOut->setShortcut(QKeySequence::ZoomOut);
    resetZoom->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_0));
    fullScreen->setShortcut(QKeySequence::FullScreen);

    connect(newTab, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->addTab(); });
    connect(newWindow, &QAction::triggered, this, [] { appManager->createWindow(); });
    connect(newPrivateWindow, &QAction::triggered, this, [] { QProcess::startDetached(QSL("crusta -p")); });
    connect(openFile, &QAction::triggered, this, [] { appManager->currentWindow()->openFile(); });
    connect(savePage, &QAction::triggered, this, [] {
        QPrinter printer;
        QPageSetupDialog dialog(&printer);
        if (dialog.exec() != QPageSetupDialog::Accepted) {
            return ;
        }

        QWebEnginePage *page = appManager->currentWindow()->tabWidget()->currentTab()->webView()->page();
        QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        QDir downloadDir(downloadPath);
        page->printToPdf(downloadDir.filePath(page->title()), printer.pageLayout());
    });

    connect(statusBar, &QAction::triggered, this, [](bool checked) {
        if (checked) {
            appManager->settings()->setValue(QSL("browserWindow/showStatusBar"), true);
        } else {
            appManager->settings()->setValue(QSL("browserWindow/showStatusBar"), false);
        }

        for (BrowserWindow *window : appManager->windows()) {
            window->statusBar()->setVisible(checked);
        }
    });
    connect(zoomIn, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->currentTab()->webView()->zoomIn(); });
    connect(zoomOut, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->currentTab()->webView()->zoomOut(); });
    connect(resetZoom, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->currentTab()->webView()->resetZoom(); });
    connect(pageSource, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->currentTab()->webView()->triggerPageAction(QWebEnginePage::ViewSource); });
    connect(fullScreen, &QAction::triggered, this, [] {
        if (appManager->currentWindow()->isFullScreen()) {
            appManager->currentWindow()->showNormal();
        } else {
            appManager->currentWindow()->showFullScreen();
        }
    });

    connect(showAllHistory, &QAction::triggered, this, [] { appManager->manager()->showHistoryManager(); });
    connect(clearAllCookies, &QAction::triggered, this, [] {
        QMessageBox::StandardButton answer = QMessageBox::question(appManager->currentWindow(), QSL("Confirm?"), QSL("Delete All Cookies?"));
        if (answer != QMessageBox::Yes) {
            return ;
        }

        appManager->webEngineProfile()->cookieStore()->deleteAllCookies();
    });
    connect(clearAllHistory, &QAction::triggered, this, [] {
        QMessageBox::StandardButton answer = QMessageBox::question(appManager->currentWindow(), QSL("Confirm?"), QSL("Delete All History?"));
        if (answer != QMessageBox::Yes) {
            return ;
        }

        appManager->dataBase()->removeAllHistory();
    });

    connect(showAllBookmarks, &QAction::triggered, this, [] { appManager->manager()->showBookmarksManager(); });
    connect(showAllDownloads, &QAction::triggered, this, [] { appManager->manager()->showDownloadsManager(); });

    connect(preferences, &QAction::triggered, this, [] { appManager->preferences()->show(); });
    connect(about, &QAction::triggered, this, [] { appManager->aboutCrusta(); });
}
