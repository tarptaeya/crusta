#pragma once

#include <QObject>

class EWindows : public QObject
{
    Q_OBJECT
public:
    explicit EWindows(QObject *parent = nullptr);

    Q_INVOKABLE void create();
};
