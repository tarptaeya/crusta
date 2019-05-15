#pragma once

#include <QObject>

struct Engine
{
    QString name;
    QString description;
    QString url;
    QString favicon;
};

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(QObject *parent = nullptr);

    static QList<Engine> knownEngines();
    static void makeDefault(const Engine &engine);
    static Engine defaultEngine();

    Q_INVOKABLE static QString searchUrlFromQuery(const QString &query);

    Q_INVOKABLE void openSearchFound(const QString &name, const QString &description, const QString &url, const QString &favicon);
    Q_INVOKABLE QString defaultSearchEngineFaviconUrl();

Q_SIGNALS:
    void engineFound(QWidget *widget, Engine engine);

private:
    bool isAlreadyPresent(const QString &name);

    static QString convertEngineToString(const Engine &engine);
    static Engine convertStringToEngine(const QString &string);

    void modifyEngineUrl(const Engine &engine);

    static Engine originalDefaultEngine();
    static QList<Engine> originalKnownEngines();

    static Engine ecosia();
    static Engine google();
};
