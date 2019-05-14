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

    Q_INVOKABLE void openSearchFound(const QString &name, const QString &description, const QString &url, const QString &favicon);

Q_SIGNALS:
    void engineFound(QWidget *widget, Engine engine);

private:
    bool isAlreadyPresent(const QString &name);

    static QString convertEngineToString(const Engine &engine);
    static Engine convertStringToEngine(const QString &string);
};
