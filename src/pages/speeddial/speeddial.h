#pragma once

#include <QObject>

class Speeddial : public QObject
{
    Q_OBJECT
public:
    explicit Speeddial(QObject *parent = nullptr);
};
