#ifndef CR_JS_OBJECT_H
#define CR_JS_OBJECT_H

#include <QObject>

class WebPage;

class JsObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* searchEngine READ searchEngine CONSTANT)
public:
    explicit JsObject(WebPage *page, QObject *parent = nullptr);

private:
    QObject *searchEngine();

    WebPage *m_page = nullptr;
    QObject *m_searchEngine = nullptr;
};

#endif
