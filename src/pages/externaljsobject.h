#pragma once

#include "speeddial/speeddial.h"
#include <QObject>
#include <QWebChannel>
#include <QWebEnginePage>

class ExternalJsObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* speeddial READ speeddial CONSTANT)
public:
    explicit ExternalJsObject(QObject *parent, QWebEnginePage *page);
    static void setUpWebChannel(QWebChannel *channel, QWebEnginePage *page);
    QObject *speeddial() const;
private:
    Speeddial *m_speeddial = nullptr;
};
