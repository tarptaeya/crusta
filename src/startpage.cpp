#include "startpage.h"

#include <QCursor>
#include <QDialog>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>

#include <QDebug>

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

void StartPage::dialSettingPopup()
{
    QWidget *widget = new QWidget;
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setWindowFlag(Qt::Popup);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    widget->setLayout(vboxLayout);

    QPushButton *bgChooser = new QPushButton(QStringLiteral("Change Background"));
    QPushButton *bgRemove = new QPushButton(QStringLiteral("Remove Background"));

    vboxLayout->addWidget(bgChooser);
    vboxLayout->addWidget(bgRemove);

    QPoint pos = QCursor::pos();
    widget->move(pos.x() - widget->sizeHint().width(), pos.y());
    widget->show();

    connect(bgChooser, &QPushButton::clicked, [this] {
        QUrl url = QFileDialog::getOpenFileUrl(nullptr, QString(), QDir::homePath());

        QSettings settings;
        settings.setValue(QStringLiteral("speeddial/background"), url.toString());

        emit reloadRequested();
    });

    connect(bgRemove, &QPushButton::clicked, [this] {
        QSettings settings;
        settings.setValue(QStringLiteral("speeddial/background"), QStringLiteral(""));

        emit reloadRequested();
    });
}

QString StartPage::background()
{
    QSettings settings;
    return settings.value(QStringLiteral("speeddial/background")).toString();
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
