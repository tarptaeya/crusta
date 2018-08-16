#include "tabbar.h"
#include "tabbarbutton.h"
#include "tab.h"
#include "omnibar/omnibar.h"
#include "../utils/dimensions.h"
#include "../webview/webview.h"

TabBar::TabBar(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setFixedHeight(Dimensions::tabBarHeight());

    m_hBoxLayout = new QHBoxLayout(this);
    setLayout(m_hBoxLayout);
    m_hBoxLayout->setContentsMargins(0, 0, 0, 0);
    m_hBoxLayout->setSpacing(0);

    m_backButton = new TabBarButton(this);
    m_forwardButton = new TabBarButton(this);
    m_stopReloadButton = new TabBarButton(this);
    m_homeButton = new TabBarButton(this);
    m_newTabButton = new TabBarButton(this);
    m_tabListButton = new TabBarButton(this);
    m_optionsButton = new TabBarButton(this);

    m_backButton->setIcon(QIcon(":/back.svg"));
    m_forwardButton->setIcon(QIcon(":/forward.svg"));
    m_stopReloadButton->setIcon(QIcon(":/refresh.svg"));
    m_homeButton->setIcon(QIcon(":/home.svg"));
    m_newTabButton->setIcon(QIcon(":/add.svg"));
    m_tabListButton->setIcon(QIcon(":/list.svg"));
    m_optionsButton->setIcon(QIcon(":/options.svg"));

    m_omniBar = new OmniBar(this);

    m_hBoxLayout->addWidget(m_backButton);
    m_hBoxLayout->addWidget(m_forwardButton);
    m_hBoxLayout->addWidget(m_stopReloadButton);
    m_hBoxLayout->addWidget(m_homeButton);
    m_hBoxLayout->addWidget(m_omniBar);
    m_hBoxLayout->addWidget(m_newTabButton);
    m_hBoxLayout->addWidget(m_tabListButton);
    m_hBoxLayout->addWidget(m_optionsButton);
}

void TabBar::setVirtualTab(QWidget *widget)
{
    destroyVirtualConnections();
    Tab *tab = qobject_cast<Tab *>(widget);
    if (!tab) {
        qDebug() << "Unable to cast QWidget * to Tab *";
        return;
    }
    m_virtualTab = tab;
    createVirtualConnections();
}

void TabBar::createVirtualConnections()
{
    const QMetaObject::Connection connection1 = connect(m_backButton, &TabBarButton::clicked, this, [this]{
        if (!m_virtualTab) {
            return;
        }
        m_virtualTab->webview()->back();
    });
    const QMetaObject::Connection connection2 = connect(m_forwardButton, &TabBarButton::clicked, this, [this]{
        if (!m_virtualTab) {
            return;
        }
        m_virtualTab->webview()->forward();
    });
    const QMetaObject::Connection connection3 = connect(m_virtualTab->webview(), &WebView::loadStarted, [this]{
        m_stopReloadButton->setIcon(QIcon(":/close.svg"));
        m_stopReloadButton->setData(QVariant::fromValue(QStringLiteral("STOP")));
    });
    const QMetaObject::Connection connection4 = connect(m_virtualTab->webview(), &WebView::loadFinished, [this]{
        m_stopReloadButton->setIcon(QIcon(":/refresh.svg"));
        m_stopReloadButton->setData(QVariant::fromValue(QStringLiteral("RELOAD")));
    });
    const QMetaObject::Connection connection5 = connect(m_stopReloadButton, &TabBarButton::clicked, [this]{
        if (m_stopReloadButton->data().toString() == QStringLiteral("STOP")) {
            m_virtualTab->webview()->stop();
        } else {
            m_virtualTab->webview()->reload();
        }
    });

    m_connections.append(connection1);
    m_connections.append(connection2);
    m_connections.append(connection3);
    m_connections.append(connection4);
    m_connections.append(connection5);
}

void TabBar::destroyVirtualConnections()
{
    while (m_connections.length() > 0) {
        const QMetaObject::Connection connection = m_connections.front();
        QObject::disconnect(connection);
        m_connections.pop_front();
    }
}
