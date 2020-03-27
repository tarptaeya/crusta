#pragma once

#include <QObject>
#include <QVector>
#include <QJSValue>
#include <QWebEnginePage>

class PluginInterface
{
public:
    virtual ~PluginInterface() {}

    virtual bool load() = 0;
    virtual void unload() = 0;
    virtual bool accept_navigation_request(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame) { return true; }
};

Q_DECLARE_INTERFACE(PluginInterface, "Crusta.PluginInterface")

#define PLUGIN_PROPERTY(name) \
    Q_PROPERTY(QJSValue name READ read_##name WRITE set_##name) \
    QJSValue m_##name; \
    QJSValue read_##name() const { return m_##name; } \
    void set_##name(const QJSValue &value) { m_##name = value; } \

class QmlPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    PLUGIN_PROPERTY(load)
    PLUGIN_PROPERTY(unload)
    PLUGIN_PROPERTY(accept_navigation_request)
public:
    explicit QmlPlugin(QObject *parent = nullptr);

    bool load() override;
    void unload() override;
    bool accept_navigation_request(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame) override;
};

class Plugins : public QObject
{
    QVector<PluginInterface *> m_plugins;

    void load_plugin(const QString &path);
    void install_qml_types();
public:
    Plugins();
    ~Plugins();

    QVector<PluginInterface *>::const_iterator begin() const;
    QVector<PluginInterface *>::const_iterator end() const;
};
