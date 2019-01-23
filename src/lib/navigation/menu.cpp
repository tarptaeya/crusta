#include "common-defs.h"
#include "browserwindow.h"
#include "mainapplication.h"
#include "manager.h"
#include "menu.h"
#include "statusbar.h"
#include "tab.h"
#include "tabwidget.h"
#include "webpage.h"
#include "webview.h"

#include <QAction>

Menu::Menu(QWidget *parent)
    : QMenu (parent)
{
    setUpMenu();
}

void Menu::setUpMenu()
{
    QAction *newTab = new QAction(QSL("New Tab"));
    QAction *newWindow = new QAction(QSL("New Window"));
    QAction *newPrivateWindow = new QAction(QSL("New Private Window"));
    QAction *openFile = new QAction(QSL("Open File"));
    QAction *savePage = new QAction(QSL("Save Page"));
    QAction *pringPage = new QAction(QSL("Print"));

    QMenu *edit = new QMenu(QSL("Edit"));
    QAction *cut = new QAction(QSL("Cut"));
    QAction *copy = new QAction(QSL("Copy"));
    QAction *paste = new QAction(QSL("Paste"));
    QAction *undo = new QAction(QSL("Undo"));
    QAction *redo = new QAction(QSL("Redo"));
    edit->addAction(cut);
    edit->addAction(copy);
    edit->addAction(paste);
    edit->addSeparator();
    edit->addAction(undo);
    edit->addAction(redo);

    QMenu *view = new QMenu(QSL("View"));
    QAction *statusBar = new QAction(QSL("Status Bar"));
    QAction *zoomIn = new QAction(QSL("Zoom In"));
    QAction *zoomOut = new QAction(QSL("Zoom Out"));
    QAction *resetZoom = new QAction(QSL("Reset Zoom"));
    QAction *pageSource = new QAction(QSL("Page Source"));
    QAction *fullScreen = new QAction(QSL("Show Full Screen"));
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
    history->addAction(showAllHistory);

    QMenu *bookmarks = new QMenu(QSL("Bookmarks"));
    QAction *showAllBookmarks = new QAction(QSL("Show All Bookmarks"));
    bookmarks->addAction(showAllBookmarks);

    QMenu *downloads = new QMenu(QSL("Downloads"));
    QAction *showAllDownloads = new QAction(QSL("Show All Downloads"));
    downloads->addAction(showAllDownloads);

    QAction *preferences = new QAction(QSL("Preferences"));
    QAction *about = new QAction(QSL("About Crusta"));
    QAction *help = new QAction(QSL("Help"));

    statusBar->setCheckable(true);

    addAction(newTab);
    addAction(newWindow);
    addAction(newPrivateWindow);
    addSeparator();
    addAction(openFile);
    addAction(savePage);
    addAction(pringPage);
    addSeparator();
    addMenu(edit);
    addMenu(view);
    addMenu(history);
    addMenu(bookmarks);
    addMenu(downloads);
    addSeparator();
    addAction(preferences);
    addAction(about);
    addAction(help);

    connect(newTab, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->addTab(); });
    connect(newWindow, &QAction::triggered, this, [] { appManager->createWindow(); });

    connect(cut, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->currentTab()->webView()->triggerPageAction(WebPage::Cut); });
    connect(copy, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->currentTab()->webView()->triggerPageAction(WebPage::Copy); });
    connect(paste, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->currentTab()->webView()->triggerPageAction(WebPage::Paste); });
    connect(undo, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->currentTab()->webView()->triggerPageAction(WebPage::Undo); });
    connect(redo, &QAction::triggered, this, [] { appManager->currentWindow()->tabWidget()->currentTab()->webView()->triggerPageAction(WebPage::Redo); });

    connect(showAllHistory, &QAction::triggered, this, [] { appManager->manager()->showHistoryManager(); });
    connect(showAllBookmarks, &QAction::triggered, this, [] { appManager->manager()->showBookmarksManager(); });
    connect(showAllDownloads, &QAction::triggered, this, [] { appManager->manager()->showDownloadsManager(); });

    connect(about, &QAction::triggered, this, [] { appManager->aboutCrusta(); });
}
