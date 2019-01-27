#ifndef CR_PLUGIN_INTERFACE_H
#define CR_PLUGIN_INTERFACE_H

#include <QJSValue>
#include <QObject>
#include <QWebEnginePage>

class PluginInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJSValue load READ readLoad WRITE writeLoad)
    Q_PROPERTY(QJSValue unload READ readUnLoad WRITE writeUnLoad)
    Q_PROPERTY(QJSValue acceptNavigationRequest READ readAcceptNavigationRequest WRITE writeAcceptNavigationRequest)
public:
    explicit PluginInterface(QObject *parent = nullptr);
    virtual ~PluginInterface() { }

    void callLoad();
    bool callAcceptNavigationRequest(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame);

private:
    QJSValue readLoad() const;
    void writeLoad(const QJSValue &jsValue);

    QJSValue readUnLoad() const;
    void writeUnLoad(const QJSValue &jsValue);

    QJSValue readAcceptNavigationRequest() const;
    void writeAcceptNavigationRequest(const QJSValue &jsValue);

    QJSValue load;
    QJSValue unload;
    QJSValue acceptNavigationRequest;
};

#endif
