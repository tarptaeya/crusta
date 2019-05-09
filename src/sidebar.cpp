#include "browser.h"
#include "history.h"
#include "sidebar.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QVBoxLayout>

SideBar::SideBar(QWidget *parent)
    : QWidget (parent)
{
    m_comboBox = new QComboBox;
    m_widget = new QStackedWidget;

    QToolButton *closeButton = new QToolButton;
    closeButton->setAutoRaise(true);
    closeButton->setIcon(QIcon::fromTheme(QStringLiteral("window-close")));
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    hboxLayout->addWidget(m_comboBox);
    hboxLayout->addWidget(closeButton);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(5, 5, 0, 5);
    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(m_widget);
    setLayout(vboxLayout);

    addHistoryItem();

    connect(closeButton, &QToolButton::clicked, this, [this] {
        hide();
    });
}

void SideBar::addItem(const QString &title, QWidget *widget)
{
    m_comboBox->addItem(title);
    m_widget->addWidget(widget);
}

void SideBar::addHistoryItem()
{
    addItem(QStringLiteral("History"), Browser::instance()->history()->historyWidget());
}

void SideBar::addBookmarksItem()
{
    addItem(QStringLiteral("Bookmarks"), new QWidget);
}
