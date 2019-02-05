#include "browserwindow.h"
#include "mainapplication.h"
#include "omnibar.h"
#include "searchengine.h"
#include "searchenginemanager.h"
#include "tab.h"
#include "tabwidget.h"
#include "toolbar.h"
#include "toolbarbutton.h"
#include "webview.h"

#include <QDebug>

SearchEngine::SearchEngine(WebPage *page, QObject *parent)
    : QObject (parent)
    , m_page(page)
{
}

void SearchEngine::openSearchData(const QString &name, const QString &queryUrl, const QString &suggestionUrl)
{
    if (appManager->searchEngineManager()->engines().contains(name)) {
        return;
    }

    for (BrowserWindow *window : appManager->windows()) {
        TabWidget *tabWidget = window->tabWidget();
        int count = tabWidget->count();
        for (int i = 0; i < count; i++) {
            if (tabWidget->tabAt(i)->webView()->page() == m_page) {
                ToolBarButton *button = tabWidget->tabAt(i)->toolBar()->addEngineButton();
                button->setVisible(true);
                button->setData(QStringList() << name << queryUrl << suggestionUrl);
                button->setToolTip(QSL("Add %1 as Search Engine").arg(name));
                return;
            }
        }
    }
}
