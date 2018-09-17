#pragma once

#include <QObject>
#include <QWebEnginePage>

class SpeeddialItem;

class Speeddial : public QObject
{
    Q_OBJECT
public:
    explicit Speeddial(QObject *parent, QWebEnginePage *page);
    Q_INVOKABLE void addDial();
    Q_INVOKABLE void removeDial(const QString &url);
    Q_INVOKABLE void loadDialsFromDatabase();
    static QList<SpeeddialItem> speeddialItems();
Q_SIGNALS:
    void dialAdded(QVariantMap map);
    void dialsAdded(QVariantList dials);
private:
    QWebEnginePage *m_page = nullptr;
    static QList<SpeeddialItem> m_speeddialItems;

    void saveDialToDatabase(const QString &title, const QString &url);
};
