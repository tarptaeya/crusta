#ifndef CR_PLUGIN_INTERFACE_H
#define CR_PLUGIN_INTERFACE_H

#include <QJSValue>
#include <QObject>
#include <QQmlListProperty>
#include <QWebEnginePage>

class PluginInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJSValue load READ readLoad WRITE writeLoad)
    Q_PROPERTY(QJSValue unload READ readUnLoad WRITE writeUnLoad)
    Q_PROPERTY(QJSValue acceptNavigationRequest READ readAcceptNavigationRequest WRITE writeAcceptNavigationRequest)
    Q_PROPERTY(QJSValue acceptDownloadRequest READ readAcceptDownloadRequest WRITE writeAcceptDownloadRequest)
    Q_PROPERTY(QQmlListProperty<QObject> childItems READ readChildItems)
    Q_CLASSINFO("DefaultProperty", "childItems")

public:
    explicit PluginInterface(QObject *parent = nullptr);
    virtual ~PluginInterface() { }

    void callLoad();
    void callUnload();
    bool callAcceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame);
    void callAcceptDownloadRequest(QWebEngineDownloadItem *download);

private:
    QJSValue readLoad() const;
    void writeLoad(const QJSValue &jsValue);

    QJSValue readUnLoad() const;
    void writeUnLoad(const QJSValue &jsValue);

    QJSValue readAcceptNavigationRequest() const;
    void writeAcceptNavigationRequest(const QJSValue &jsValue);

    QJSValue readAcceptDownloadRequest() const;
    void writeAcceptDownloadRequest(const QJSValue &jsValue);

    QQmlListProperty<QObject> readChildItems();

    QJSValue load;
    QJSValue unload;
    QJSValue acceptNavigationRequest;
    QJSValue acceptDownloadRequest;

    QList<QObject *> childItems;
};

#endif
