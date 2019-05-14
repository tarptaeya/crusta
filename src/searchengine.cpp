#include "searchengine.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

SearchEngine::SearchEngine(QObject *parent)
    : QObject (parent)
{

}

void SearchEngine::openSearchFound(const QString &name, const QString &description, const QString &url)
{
    if (name.isEmpty() || url.isEmpty()) {
        return;
    }

    Engine engine;
    engine.name = name;
    engine.description = description;
    engine.url = url;

    QWidget *widget = new QWidget;
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    widget->setLayout(vboxLayout);

    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    QLabel *titleLabel = new QLabel(QStringLiteral("<b>Add %1 to Crusta?</b>").arg(name));
    QLabel *descriptionLabel = new QLabel(QStringLiteral("%2").arg(description));
    QPushButton *nopeButton = new QPushButton(QStringLiteral("Nope"));
    QPushButton *okButton = new QPushButton(QStringLiteral("Add"));

    descriptionLabel->setWordWrap(true);
    okButton->setObjectName(QStringLiteral("default"));

    vboxLayout->addWidget(titleLabel);
    vboxLayout->addWidget(descriptionLabel);
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(nopeButton);
    hboxLayout->addWidget(okButton);
    vboxLayout->addLayout(hboxLayout);

    connect(nopeButton, &QPushButton::clicked, [widget] { widget->close(); });
    connect(okButton, &QPushButton::clicked, [widget] { widget->close(); });

    emit engineFound(widget, engine);
}
