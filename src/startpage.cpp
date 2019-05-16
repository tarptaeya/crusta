#include "startpage.h"
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

StartPage::StartPage(QObject *parent)
    : QObject (parent)
{
}

QJsonObject StartPage::newDialPopup()
{
    QDialog widget;
    widget.setWindowFlag(Qt::FramelessWindowHint);
    QLabel titleLabel(QStringLiteral("Title"));
    QLineEdit title;
    QLabel addressLabel(QStringLiteral("Address"));
    QLineEdit address;
    QPushButton save(QStringLiteral("Save"));
    QPushButton cancel(QStringLiteral("Cancel"));

    save.setDefault(true);

    QGridLayout grid;
    grid.addWidget(&titleLabel, 1, 1);
    grid.addWidget(&title, 1, 2);
    grid.addWidget(&addressLabel, 2, 1);
    grid.addWidget(&address, 2, 2);

    QHBoxLayout hboxLayout;
    hboxLayout.addWidget(&cancel);
    hboxLayout.addWidget(&save);
    grid.addLayout(&hboxLayout, 3, 2);

    widget.setLayout(&grid);

    connect(&cancel, &QPushButton::clicked, [&widget] { widget.reject(); });
    connect(&save, &QPushButton::clicked, [&widget] { widget.accept(); });
    if (widget.exec() == QDialog::Rejected) {
        return QJsonObject();
    }

    QJsonObject object;
    object.insert(QStringLiteral("title"), title.text());
    object.insert(QStringLiteral("address"), address.text());
    return object;
}
