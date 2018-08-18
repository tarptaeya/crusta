#pragma once

#include "speeddial/speeddial.h"
#include <QObject>
#include <QWebChannel>

class ExternalJsObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* speeddial READ speeddial CONSTANT)
public:
    explicit ExternalJsObject(QObject *parent = nullptr);
    static QWebChannel *setUpWebChannel();
    QObject *speeddial() const;
private:
    Speeddial *m_speeddial = nullptr;
};
