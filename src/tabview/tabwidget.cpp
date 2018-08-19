#include "tabwidget.h"
#include "tabbar.h"
#include "tab.h"
#include "stackedwidget.h"
#include "tablist.h"
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
    splitter->addWidget(m_tabList);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    vBoxLayout->addWidget(splitter);
    setLayout(vBoxLayout);

    connect(m_stakedWidget, &StackedWidget::currentChanged, this, [this]{
        m_tabBar->setVirtualTab(m_stakedWidget->currentWidget());
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
    Tab *tab = new Tab(m_stakedWidget);
    if (!url.isEmpty()) {
        tab->webview()->load(url);
    } else {
        tab->webview()->loadStartupUrl();
    }
    int index = m_stakedWidget->addWidget(tab);
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
