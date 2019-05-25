#pragma once

#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QSettings>
#include <QWebEnginePage>

class Plugin : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJSValue create READ create WRITE setCreate CONSTANT)
    Q_PROPERTY(QJSValue destroy READ destroy WRITE setDestroy CONSTANT)
    Q_PROPERTY(QJSValue acceptNavigationRequest READ acceptNavigationRequest WRITE setAcceptNavigationRequest CONSTANT)
public:
    explicit Plugin(QObject *parent = nullptr);
    Plugin *createPlugin(const QString &path);

    QSettings *metadata();
    void setMetadata(const QString &path);

    void createFn();
    void destroyFn();
    bool acceptNavigationRequestFn(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame);

private:
    QQmlEngine *m_engine = nullptr;
    QQmlComponent *m_component = nullptr;
    QObject *m_object = nullptr;

    QSettings *m_metadata = nullptr;

    QJSValue m_create;
    QJSValue m_destroy;
    QJSValue m_acceptNavigationRequest;

    QJSValue create();
    void setCreate(QJSValue value);

    QJSValue destroy();
    void setDestroy(QJSValue value);

    QJSValue acceptNavigationRequest();
    void setAcceptNavigationRequest(QJSValue value);
};
