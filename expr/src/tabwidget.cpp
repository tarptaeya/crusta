#include "tab.h"
#include "tabwidget.h"

#include <QTabBar>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget (parent)
{
    m_newTabButton = new QToolButton;

    setDocumentMode(true);
    setMovable(true);
    setTabsClosable(true);

    tabBar()->setExpanding(true);

    setContentsMargins(0, 0, 0, 0);

    setCornerWidget(m_newTabButton);

    addTab(new Tab, QStringLiteral("New Tab"));

    connect(m_newTabButton, &QToolButton::clicked, this, [this] { addTab(new Tab, QStringLiteral("New Tab")); });
    connect(this, &TabWidget::tabCloseRequested, this, [this] (int index) {
        Tab *tab = dynamic_cast<Tab *>(widget(index));
        removeTab(index);
        tab->deleteLater();

        if (!count()) {
            // TODO: add option to not close window when
            //       last tab is closed
            emit windowCloseRequested();
        }
    });
}
