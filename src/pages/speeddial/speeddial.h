#pragma once

#include <QObject>
#include <QWebEnginePage>

class Speeddial : public QObject
{
    Q_OBJECT
public:
    explicit Speeddial(QObject *parent, QWebEnginePage *page);
    Q_INVOKABLE void addDial();
    Q_INVOKABLE void loadDialsFromDatabase();
Q_SIGNALS:
    void dialAdded(const QString &url);
    void dialsAdded(QVariantList dials);
private:
    QWebEnginePage *m_page = nullptr;

    void saveDialToDatabase(const QString &url);
};
