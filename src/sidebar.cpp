#include "browser.h"
#include "history.h"
#include "sidebar.h"

#include <QHBoxLayout>
#include <QSettings>
#include <QToolButton>
#include <QVBoxLayout>

SideBar::SideBar(QWidget *parent)
    : QWidget (parent)
{
    setObjectName(QStringLiteral("sidebar"));

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

    connect(closeButton, &QToolButton::clicked, this, [this] {
        QSettings settings;
        settings.setValue(QStringLiteral("appearance/sidebar"), false);

        hide();
    });
    connect(m_comboBox, &QComboBox::currentTextChanged, this, [this] { m_widget->setCurrentIndex(m_comboBox->currentIndex()); });
}

void SideBar::addItem(const QString &title, QWidget *widget)
{
    m_comboBox->addItem(title);
    m_widget->addWidget(widget);
}

void SideBar::show()
{
    QWidget::show();
}

void SideBar::hide()
{
    QWidget::hide();
}
