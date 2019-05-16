#pragma once

#include <QJsonObject>
#include <QObject>

class StartPage : public QObject
{
    Q_OBJECT
public:
    explicit StartPage(QObject *parent = nullptr);

    Q_INVOKABLE void newDialPopup();
    Q_INVOKABLE void loadAllDials();

Q_SIGNALS:
    void dialAdded(const QJsonObject &dial);

private:
    void saveDial(const QString &title, const QString &url);
};
