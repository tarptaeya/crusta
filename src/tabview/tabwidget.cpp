#include "tabwidget.h"
#include "tabbar.h"
#include "tab.h"
#include "stackedwidget.h"
#include "tablist.h"
#include "tablistitem.h"
#include "../webview/webview.h"
#include <QVBoxLayout>
#include <QSplitter>
#include <QUrl>

TabWidget::TabWidget(QWidget *parent)
    : QWidget(parent)
{
    m_tabBar = new TabBar(this);
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);
    vBoxLayout->addWidget(m_tabBar);

    QSplitter *splitter = new QSplitter;
    splitter->setContentsMargins(0, 0, 0, 0);
    splitter->setMidLineWidth(0);
    splitter->setHandleWidth(0);

    m_stakedWidget = new StackedWidget(this);
    splitter->addWidget(m_stakedWidget);

    m_tabList = new TabList(this);
    m_tabList->setVirtualTabWidget(this);
    splitter->addWidget(m_tabList);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    vBoxLayout->addWidget(splitter);
    setLayout(vBoxLayout);

    connect(m_stakedWidget, &StackedWidget::currentChanged, this, [this]{
        Tab *tab = qobject_cast<Tab *>(m_stakedWidget->currentWidget());
        m_tabBar->setVirtualTab(tab);
        handleTabChanged(tab);
    });

    addTab();
}

TabWidget::~TabWidget()
{
}

void TabWidget::addTab()
{
    addTab(QUrl(""));
}

void TabWidget::addTab(const QUrl &url)
{
    WebView *webview = new WebView;
    if (!url.isEmpty()) {
        webview->load(url);
    } else {
        webview->loadStartupUrl();
    }
    addTab(webview);
}

void TabWidget::addTab(WebView *webview)
{
    int index = addBackgroundTab(webview);
    m_stakedWidget->setCurrentIndex(index);
}

int TabWidget::addBackgroundTab(WebView *webview)
{
    Tab *tab = new Tab(m_stakedWidget, webview);
    tab->setVirtualTabWidget(this);
    m_tabList->addTab(tab);
    int index = m_stakedWidget->addWidget(tab);
    return index;
}

void TabWidget::closeTab(Tab *tab)
{
    m_stakedWidget->removeWidget(tab);
    m_tabList->closeTab(tab);
    tab->closeTab();

    // FIXME: make modular
    int index = m_stakedWidget->currentIndex();
    handleTabChanged(m_tabList->tabAt(index));
}

void TabWidget::closeTabsAfter(Tab *tab)
{
    int i = m_tabList->tabs().length() - 1;
    while (i > -1) {
        Tab *otherTab = m_tabList->tabs().at(i);
        if (otherTab == tab) {
            break;
        }
        closeTab(otherTab);
        i--;
    }
}

void TabWidget::closeTabsBefore(Tab *tab)
{
    int i = 0;
    while (i < m_tabList->tabs().length()) {
        Tab *otherTab = m_tabList->tabs().at(i);
        if (otherTab == tab) {
            break;
        }
        closeTab(otherTab);
    }
}

void TabWidget::currentTabChanged(Tab *tab)
{
    int index = m_tabList->indexOf(tab);
    m_stakedWidget->setCurrentIndex(index);
}

void TabWidget::toggleTabList()
{
    if (m_tabList->isVisible()) {
        m_tabList->hide();
    } else {
        m_tabList->show();
    }
}

void TabWidget::handleTabChanged(Tab *tab)
{
    if (!tab) {
        return;
    }

    for (Tab *tab_ : m_tabList->tabs()) {
        tab_->tabListItem()->setCurrent(false);
    }
    tab->tabListItem()->setCurrent(true);
}
