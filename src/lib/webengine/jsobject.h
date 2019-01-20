#ifndef CR_JS_OBJECT_H
#define CR_JS_OBJECT_H

#include <QObject>

class JsObject : public QObject
{
public:
    explicit JsObject(QObject *parent = nullptr);
};

#endif
