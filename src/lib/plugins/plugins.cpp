#include "common-defs.h"
#include "plugininterface.h"
#include "pluginloader.h"
#include "plugins.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSettings>

Plugins::Plugins(QObject *parent)
    : QObject (parent)
{

}

Plugins::~Plugins()
{
    for (Plugin *plugin : m_plugins) {
        if (plugin->enabled) {
            plugin->enabled = false;
            plugin->interface->callUnload();
        }

        delete plugin;
    }
}

void Plugins::loadPlugins()
{
    // loadInternalPlugins();
    // loadExternalPlugins();
}

QList<Plugin *> Plugins::plugins()
{
    return m_plugins;
}

void Plugins::enablePlugin(Plugin *plugin)
{
    if (plugin->enabled) {
        return;
    }

    plugin->enabled = true;
    plugin->interface->callLoad();
}

void Plugins::disablePlugin(Plugin *plugin)
{
    if (!plugin->enabled) {
        return;
    }

    plugin->enabled = false;
    plugin->interface->callUnload();
}

void Plugins::loadInternalPlugins()
{
    loadPluginsFromPath(QSL(":/plugins"));
}

void Plugins::loadExternalPlugins()
{

}

void Plugins::loadPluginsFromPath(const QString &path)
{
    QDir dir(path);
    const QStringList pluginDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &pluginDir : qAsConst(pluginDirs)) {
        const QString actualPath = dir.filePath(pluginDir);
        Plugin *plugin = parseMetaData(actualPath);
        if (plugin->name.isEmpty() || plugin->entryFile.isEmpty()) {
            continue;
        }

        loadPlugin(plugin);
    }
}

void Plugins::loadPlugin(Plugin *plugin)
{
    PluginLoader *pluginLoader = new PluginLoader(this);
    pluginLoader->loadPlugin(plugin);
    if (!plugin->interface) {
        delete plugin;
        return;
    }

    enablePlugin(plugin);

    m_plugins.append(plugin);
}

Plugin *Plugins::parseMetaData(const QString &path)
{
    QString metaDataFile = QDir(path).filePath(QSL("metadata.desktop"));
    Plugin *plugin = new Plugin;
    plugin->path = path;

    QSettings settings(metaDataFile, QSettings::IniFormat);
    plugin->name = settings.value(QSL("name")).toString();
    plugin->description = settings.value(QSL("description")).toString();
    plugin->entryFile = settings.value(QSL("entryFile")).toString();
    plugin->iconFile = settings.value(QSL("iconFile")).toString();
    plugin->author = settings.value(QSL("author")).toString();
    plugin->email = settings.value(QSL("email")).toString();
    return plugin;
}
