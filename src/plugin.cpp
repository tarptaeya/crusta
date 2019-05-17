#include "plugin.h"
#include "utils.h"

#include <QJSValue>

Plugin::Plugin(QObject *parent)
    : QObject (parent)
{
    m_engine = new QQmlEngine(this);
}

Plugin *Plugin::createPlugin(const QString &path)
{
    m_component = new QQmlComponent(m_engine, QStringLiteral("%1/main.qml").arg(path), this);
    if (m_component->isError()) {
        qDebug() << m_component->errorString();
        return nullptr;
    }

    m_object = m_component->create();
    Plugin *plugin = dynamic_cast<Plugin *>(m_object);
    if (!plugin) {
        return nullptr;
    }

    plugin->setMetadata(path);
    return plugin;
}

QSettings *Plugin::metadata()
{
    return m_metadata;
}

void Plugin::setMetadata(const QString &path)
{
    m_metadata = new QSettings(QStringLiteral("%1/metadata.desktop").arg(path), QSettings::IniFormat, this);
}

void Plugin::createFn()
{
    if (!m_create.isCallable()) {
        return;
    }

    m_create.call();
}

void Plugin::destroyFn()
{
    if (!m_destroy.isCallable()) {
        return;
    }

    m_destroy.call();
}

QJSValue Plugin::create()
{
    return m_create;
}

void Plugin::setCreate(QJSValue value)
{
    m_create = value;
}

QJSValue Plugin::destroy()
{
    return m_destroy;
}

void Plugin::setDestroy(QJSValue value)
{
    m_destroy = value;
}
