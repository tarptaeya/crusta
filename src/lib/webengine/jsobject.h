#ifndef CR_JS_OBJECT_H
#define CR_JS_OBJECT_H

#include <QObject>

class JsObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* searchEngine READ searchEngine CONSTANT)
public:
    explicit JsObject(QObject *parent = nullptr);

private:
    QObject *searchEngine();

    QObject *m_searchEngine = nullptr;
};

#endif
