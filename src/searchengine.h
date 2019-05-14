#pragma once

#include <QObject>

struct Engine
{
    QString name;
    QString description;
    QString url;
};

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(QObject *parent = nullptr);

    Q_INVOKABLE void openSearchFound(const QString &name, const QString &description, const QString &url);

Q_SIGNALS:
    void engineFound(QWidget *widget, Engine engine);
};
