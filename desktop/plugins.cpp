#include "plugins.h"

#include <QDir>
#include <QQmlEngine>
#include <QQmlComponent>

#include <iostream>

QmlPlugin::QmlPlugin(QObject *parent)
    : QObject(parent)
{
}

bool QmlPlugin::load()
{
    return m_load.call().toBool();
}

void QmlPlugin::unload()
{
    m_unload.call();
}

bool QmlPlugin::accept_navigation_request(const QUrl &url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
    if (!m_accept_navigation_request.isCallable())
        return true;

    QJSValueList args;
    args << url.toString();
    args << type;
    args << isMainFrame;
    QJSValue value = m_accept_navigation_request.call(args);
    if (!value.isBool())
        return true;

    return value.toBool();
}

void Plugins::load_plugin(const QString &path)
{
    QQmlEngine *engine = new QQmlEngine(this);
    QQmlComponent *component = new QQmlComponent(engine, QStringLiteral("%1/main.qml").arg(path), this);
    if (component->isError()) {
        std::cerr << "[Plugin Load Error]\n";
        std::cerr << component->errorString().toStdString() << "\n";
        std::cerr << std::endl;
        return;
    }

    QObject *object = component->create();
    PluginInterface *plugin = dynamic_cast<PluginInterface *>(object);
    if (!plugin) {
        delete object;
        return;
    }

    m_plugins.append(plugin);
    plugin->load();
}

void Plugins::install_qml_types()
{
    qmlRegisterType<QmlPlugin>("Crusta", 1, 0, "PluginInterface");
}

Plugins::Plugins()
{
    install_qml_types();

    QDir plugin_dir(QStringLiteral(":plugins"));
    const QStringList plugins = plugin_dir.entryList();
    for (const QString &path : plugins) {
        load_plugin(QStringLiteral(":plugins/%1").arg(path));
    }
}

Plugins::~Plugins()
{
    for (PluginInterface *plugin : m_plugins) {
        plugin->unload();
        delete plugin;
    }
}

QVector<PluginInterface *>::const_iterator Plugins::begin() const
{
    return m_plugins.begin();
}

QVector<PluginInterface *>::const_iterator Plugins::end() const
{
    return m_plugins.end();
}
