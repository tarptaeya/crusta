#include "plugins.h"

#include <QDir>
#include <QQmlEngine>
#include <QStandardPaths>

Plugins::Plugins(QObject *parent)
    : QObject (parent)
{
    installTypes();
    loadAllPlugins();
}

Plugins::~Plugins()
{
    unloadAllPlugins();
}

void Plugins::loadPlugin(const QString &path)
{
    Plugin *prePlugin = new Plugin(this);
    Plugin *plugin = prePlugin->createPlugin(path);
    if (!plugin) {
        return;
    }

    plugin->setParent(this);
    plugin->createFn();
    m_plugins.append(plugin);
}

void Plugins::installTypes()
{
    const char *uri = "Crusta";
    qmlRegisterType<Plugin>(uri, 1, 0, "Plugin");
}

void Plugins::loadAllPlugins()
{
    QDir standardLocation(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if (!standardLocation.cd(QStringLiteral("plugins"))) {
        standardLocation.mkdir(QStringLiteral("plugins"));
        return;
    }

    const QStringList entries = standardLocation.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &entry : entries) {
        const QString path(QStringLiteral("%1/%2").arg(standardLocation.absolutePath(), entry));
        loadPlugin(path);
    }
}

void Plugins::unloadAllPlugins()
{
    for(Plugin *plugin : m_plugins) {
        plugin->destroyFn();
    }

    m_plugins.clear();
}
