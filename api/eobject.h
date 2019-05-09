#pragma once

#include <QObject>

class EWindows;

class EObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject *windows READ windows CONSTANT)

public:
    explicit EObject(QObject *parent = nullptr);

private:
    EWindows *m_ewindows;

    QObject *windows();
};
