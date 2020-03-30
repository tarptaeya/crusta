#pragma once

#include <QObject>
#include <QSettings>

class Storage;

class WebChannelObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject *storage READ storage CONSTANT)

    Storage *m_storage = nullptr;
public:
    explicit WebChannelObject(QObject *parent = nullptr);

private:
    QObject *storage() const;
};
