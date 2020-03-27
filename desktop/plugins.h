#pragma once

#include <QObject>
#include <QVector>
#include <QJSValue>

class PluginInterface
{
public:
    virtual ~PluginInterface() {}

    virtual bool load() = 0;
    virtual void unload() = 0;
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
public:
    explicit QmlPlugin(QObject *parent = nullptr);

    bool load() override;
    void unload() override;
};

class Plugins : public QObject
{
    QVector<PluginInterface *> m_plugins;

    void load_plugin(const QString &path);
    void install_qml_types();
public:
    Plugins();
    ~Plugins();
};
