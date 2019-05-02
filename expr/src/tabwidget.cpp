#include "tab.h"
#include "tabwidget.h"
#include "webview.h"

#include <QTabBar>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget (parent)
{
    m_newTabButton = new QToolButton;

    m_newTabButton->setAutoRaise(true);
    m_newTabButton->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));

    QTabBar *tabBar = this->tabBar();
    tabBar->setDocumentMode(true);
    tabBar->setExpanding(true);
    tabBar->setMovable(true);
    tabBar->setTabsClosable(true);
    tabBar->setUsesScrollButtons(true);

    setContentsMargins(0, 0, 0, 0);

    setCornerWidget(m_newTabButton);

    addTab(new Tab, QStringLiteral("New Tab"));

    connect(m_newTabButton, &QToolButton::clicked, this, [this] { addTab(new Tab, QStringLiteral("New Tab")); });
    connect(tabBar, &QTabBar::tabCloseRequested, this, [this] (int index) {
        Tab *tab = dynamic_cast<Tab *>(widget(index));
        if (!tab) {
            return;
        }

        removeTab(index);
        tab->deleteLater();

        if (!count()) {
            // TODO: add option to not close window when
            //       last tab is closed
            emit windowCloseRequested();
        }
    });
}
