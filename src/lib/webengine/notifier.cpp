#include "common-defs.h"
#include "notifier.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

Notifier::Notifier(QWidget *parent)
    : QDialog (parent)
{
    m_acceptButton = new QPushButton(QSL("Accept"));
    m_rejectButton = new QPushButton(QSL("Reject"));

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(new QWidget, 1);
    hboxLayout->addWidget(m_rejectButton);
    hboxLayout->addWidget(m_acceptButton);
    vboxLayout->addLayout(hboxLayout);
    setLayout(vboxLayout);
}
