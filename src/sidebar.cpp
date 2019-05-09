#include "history.h"
#include "sidebar.h"

#include <QVBoxLayout>

SideBar::SideBar(QWidget *parent)
    : QWidget (parent)
{
    m_comboBox = new QComboBox;
    m_widget = new QStackedWidget;

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(5, 5, 0, 5);
    vboxLayout->addWidget(m_comboBox);
    vboxLayout->addWidget(m_widget);
    setLayout(vboxLayout);

    addHistoryItem();
}

void SideBar::addItem(const QString &title, QWidget *widget)
{
    m_comboBox->addItem(title);
    m_widget->addWidget(widget);
}

void SideBar::addHistoryItem()
{
    addItem(QStringLiteral("History"), History::historyWidget());
}

void SideBar::addBookmarksItem()
{
    addItem(QStringLiteral("Bookmarks"), new QWidget);
}
