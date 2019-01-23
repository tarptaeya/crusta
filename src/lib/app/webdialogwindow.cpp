#include "omnibar.h"
#include "statusbar.h"
#include "webdialogwindow.h"
#include "webpage.h"
#include "webview.h"

#include <QKeyEvent>
#include <QVBoxLayout>

WebDialogWindow::WebDialogWindow(WebView *webView, QWidget *parent)
    : QMainWindow (parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setSpacing(0);
    vboxLayout->setContentsMargins(0, 0, 0, 0);

    OmniBar *omniBar = new OmniBar(this);
    // Make omnibar read only
    omniBar->setEnabled(false);

    // Don't show webview context menu
    webView->setContextMenuPolicy(Qt::NoContextMenu);

    StatusBar *statusBar = new StatusBar(this);

    vboxLayout->addWidget(omniBar);
    vboxLayout->addWidget(webView);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(vboxLayout);

    setCentralWidget(centralWidget);
    setStatusBar(statusBar);

    webView->setFocus();
    resize(600, 400);

    connect(webView, &WebView::urlChanged, this, [omniBar](const QUrl &url) { omniBar->update(url.toDisplayString()); });
    connect(webView->page(), &WebPage::linkHovered, this, [statusBar](const QUrl &url) { statusBar->showMessage(url.toDisplayString()); });
}
