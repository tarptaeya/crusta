#include "tablistitem.h"
#include "tab.h"
#include "tabwidget.h"
#include "../webview/webview.h"
#include "../utils/dimensions.h"
#include <QIcon>

TabListItem::TabListItem(QWidget *parent)
    : QWidget(parent)
{
    m_hBoxLayout = new QHBoxLayout;
    m_hBoxLayout->setContentsMargins(5, 0, 5, 0);
    setLayout(m_hBoxLayout);

    m_favicon = new QLabel(this);
    m_favicon->setFixedSize(20, 20); // arbitrary number greater than 16
    m_title = new QLabel(this);
    m_closeTabButton = new QClickableLabel(this);
    m_closeTabButton->setFixedSize(20, 20);
    m_closeTabButton->setText("X");

    m_hBoxLayout->addWidget(m_favicon);
    m_hBoxLayout->addWidget(m_title);
    m_hBoxLayout->addWidget(m_closeTabButton);
    setFixedHeight(Dimensions::tabListItemHeight());
}

void TabListItem::setVirtualTab(Tab *tab)
{
    m_tab = tab;
    tab->setTabListItem(this);
    connect(m_tab->webview(), &WebView::iconChanged, this, &TabListItem::setFavicon);
    connect(m_tab->webview(), &WebView::titleChanged, this, &TabListItem::setTitle);
    connect(m_closeTabButton, &QClickableLabel::clicked, this, [this]{
        if (!m_tabWidget) {
            return;
        }
        m_tabWidget->closeTab(m_tab);
    });
}

void TabListItem::setVirtualTabWidget(TabWidget *tabWidget)
{
    m_tabWidget = tabWidget;
}

void TabListItem::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    m_tabWidget->currentTabChanged(m_tab);
}

void TabListItem::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (m_tab) {
        setTitle(m_tab->webview()->title());
    }
}

void TabListItem::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);

    if (m_state == Current) {
        return;
    }

    m_state = Hovered;
    update();
}

void TabListItem::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);

    if (m_state == Current) {
        return;
    }

    m_state = Normal;
    update();
}

void TabListItem::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (m_state == Current) {
        QPainter painter(this);
        painter.fillRect(0, 0, width(), height(), QColor::fromRgb(240, 240, 240));
    } else if (m_state == Hovered) {
        QPainter painter(this);
        painter.fillRect(0, 0, width(), height(), QColor::fromRgb(247, 247, 247));
    }
}

void TabListItem::setCurrent(bool current)
{
    if (current) {
        m_state = Current;
    } else {
        m_state = Normal;
    }
    update();
}

void TabListItem::setFavicon(const QIcon &favicon)
{
    m_favicon->setPixmap(favicon.pixmap(16, 16));
}

void TabListItem::setTitle(const QString &title)
{
    const QString elidedTitle = m_title->fontMetrics().elidedText(title, Qt::ElideRight, m_title->width() - 10); // 10 is arbitrary
    m_title->setText(elidedTitle);
}
