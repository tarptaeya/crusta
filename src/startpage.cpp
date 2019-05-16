#include "startpage.h"
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>

StartPage::StartPage(QObject *parent)
    : QObject (parent)
{
}

void StartPage::newDialPopup()
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
        return;
    }

    QJsonObject object;
    object.insert(QStringLiteral("title"), title.text());
    object.insert(QStringLiteral("address"), address.text());

    saveDial(title.text(), address.text());

    emit dialAdded(object);
}

void StartPage::loadAllDials()
{
    QSettings settings;
    QStringList titles = settings.value(QStringLiteral("speeddial/titles")).toStringList();
    QStringList addresses = settings.value(QStringLiteral("speeddial/addresses")).toStringList();

    Q_ASSERT(titles.length() == addresses.length());

    for (int i = 0; i < titles.length(); i++) {
        QJsonObject dial;
        dial.insert(QStringLiteral("title"), titles[i]);
        dial.insert(QStringLiteral("address"), addresses[i]);

        emit dialAdded(dial);
    }
}

void StartPage::saveDial(const QString &title, const QString &url)
{
    QSettings settings;
    QStringList titles = settings.value(QStringLiteral("speeddial/titles")).toStringList();
    QStringList addresses = settings.value(QStringLiteral("speeddial/addresses")).toStringList();

    Q_ASSERT(titles.length() == addresses.length());

    if (addresses.contains(url)) {
        return;
    }

    titles.append(title);
    addresses.append(url);

    settings.setValue(QStringLiteral("speeddial/titles"), titles);
    settings.setValue(QStringLiteral("speeddial/addresses"), addresses);
}
