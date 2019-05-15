#pragma once

#include "searchengine.h"
#include "startpage.h"

#include <QObject>

class WebObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject *searchEngine READ searchEngine CONSTANT)
    Q_PROPERTY(QObject *startPage READ startPage CONSTANT)
public:
    explicit WebObject(QObject *parent = nullptr);

Q_SIGNALS:
    void popupRequested(QWidget *widget);

private:
    SearchEngine *m_searchEngine = nullptr;
    StartPage *m_startPage = nullptr;

    QObject *searchEngine();
    QObject *startPage();
};
