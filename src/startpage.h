#pragma once

#include <QObject>

class StartPage : public QObject
{
    Q_OBJECT
public:
    explicit StartPage(QObject *parent = nullptr);
};
