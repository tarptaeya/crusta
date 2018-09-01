#pragma once

#include <QObject>
#include <QWebEnginePage>

class Speeddial : public QObject
{
    Q_OBJECT
public:
    explicit Speeddial(QObject *parent, QWebEnginePage *page);
    Q_INVOKABLE void addDial();
Q_SIGNALS:
    void dialAdded(const QString &url);
private:
    QWebEnginePage *m_page = nullptr;
};
