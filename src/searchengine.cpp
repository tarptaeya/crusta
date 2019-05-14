#include "searchengine.h"

#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include <QWidget>

SearchEngine::SearchEngine(QObject *parent)
    : QObject (parent)
{
}

void SearchEngine::openSearchFound(const QString &name, const QString &description, const QString &url, const QString &favicon)
{
    if (name.isEmpty() || url.isEmpty() || isAlreadyPresent(name)) {
        return;
    }

    Engine engine;
    engine.name = name;
    engine.description = description;
    engine.url = url;
    engine.favicon = favicon;

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
    connect(okButton, &QPushButton::clicked, [widget, engine] {
        QSettings settings;
        QStringList engines = settings.value(QStringLiteral("searchengine/engines"), QStringList()).toStringList();
        engines.append(SearchEngine::convertEngineToString(engine));
        settings.setValue(QStringLiteral("searchengine/engines"), engines);
        widget->close();
    });

    emit engineFound(widget, engine);
}

bool SearchEngine::isAlreadyPresent(const QString &name)
{
    QSettings settings;
    QStringList engines = settings.value(QStringLiteral("searchengine/engines")).toStringList();
    for (const QString &engineString : engines) {
        Engine engine = convertStringToEngine(engineString);
        if (engine.name == name) {
            return true;
        }
    }
    return false;
}

QString SearchEngine::convertEngineToString(const Engine &engine)
{
    // FIXME: TODO: (but, I am pretty sure (ahem) that __CR-SEPERATOR__ will not be present in name, desc, url or favicon)
    return QStringLiteral("%1__CR-SEPERATOR__%2__CR-SEPERATOR__%3__CR-SEPERATOR__%4")
            .arg(engine.name).arg(engine.description).arg(engine.url).arg(engine.favicon);
}

Engine SearchEngine::convertStringToEngine(const QString &string)
{
    QStringList list = string.split(QStringLiteral("__CR-SEPERATOR__"));
    Engine engine;
    engine.name = list[0];
    engine.description = list[1];
    engine.url = list[2];
    engine.favicon = list[3];
    return engine;
}
