#pragma once

#include <QJsonObject>
#include <QObject>

class StartPage : public QObject
{
    Q_OBJECT
public:
    explicit StartPage(QObject *parent = nullptr);

    Q_INVOKABLE QJsonObject newDialPopup();
};
