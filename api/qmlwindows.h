#pragma once

#include <QJsonObject>
#include <QObject>

class QmlWindow : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url CONSTANT)
public:
    explicit QmlWindow(QObject *parent = nullptr);

    QString url() const;
    void setUrl(const QString &url);

private:
    QString m_url;
};

class QmlWindows : public QObject
{
    Q_OBJECT
public:
    explicit QmlWindows(QObject *parent = nullptr);
    Q_INVOKABLE QmlWindow *create(QJsonObject param = QJsonObject());
};
