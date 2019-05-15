#include "searchengine.h"

#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

SearchEngine::SearchEngine(QObject *parent)
    : QObject (parent)
{
}

QList<Engine> SearchEngine::knownEngines()
{
    QList<Engine> engines(originalKnownEngines());

    QSettings settings;
    QStringList enginesString = settings.value(QStringLiteral("searchengine/engines")).toStringList();
    for (const QString &engineString : enginesString) {
        Engine engine = convertStringToEngine(engineString);
        engines.append(engine);
    }

    return engines;
}

void SearchEngine::makeDefault(const Engine &engine)
{
    QSettings settings;
    settings.setValue(QStringLiteral("searchengine/default"), convertEngineToString(engine));
}

Engine SearchEngine::defaultEngine()
{
    QSettings settings;
    QString engine = settings.value(QStringLiteral("searchengine/default"), convertEngineToString(originalDefaultEngine())).toString();
    return convertStringToEngine(engine);
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

QString SearchEngine::defaultSearchEngineFaviconUrl()
{
    Engine engine = defaultEngine();
    QUrl url(engine.url);
    return url.resolved(engine.favicon).toString();
}

bool SearchEngine::isAlreadyPresent(const QString &name)
{
    QList<Engine> engines = knownEngines();
    for (const Engine &engine : engines) {
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

Engine SearchEngine::originalDefaultEngine()
{
    return ecosia();
}

QList<Engine> SearchEngine::originalKnownEngines()
{
    QList<Engine> engines;
    engines.append(ecosia());
    engines.append(google());

    return engines;
}

Engine SearchEngine::ecosia()
{
    return { QStringLiteral("Ecosia"),
             QStringLiteral("Search the web to plant trees"),
             QStringLiteral("https://www.ecosia.org/search?tt=crusta&q={searchTerms}"),
             QStringLiteral("https://cdn.ecosia.org/assets/images/ico/favicon.ico")};
}

Engine SearchEngine::google()
{
    return { QStringLiteral("Google"),
             QStringLiteral(""),
             QStringLiteral("https://google.com/search?client=crusta&q={searchTerms}"),
             QStringLiteral("https://google.com/favicon.ico")};
}
