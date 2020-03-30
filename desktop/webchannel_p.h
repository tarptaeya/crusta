#pragma once

#include <QObject>
#include <QSettings>

class Storage : public QObject
{
    Q_OBJECT

    QSettings m_settings;
public:
    explicit Storage(QObject *parent = nullptr);

    Q_INVOKABLE QVariant get(const QString &string) const;
    Q_INVOKABLE void set(const QString &string, const QVariant &value);
};
