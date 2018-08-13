#pragma once

#include <QObject>
#include <QSettings>

#define APPEARANCE QStringLiteral("appearance")

class Settings : public QSettings
{
public:
    explicit Settings(QObject *parent = nullptr);
};
