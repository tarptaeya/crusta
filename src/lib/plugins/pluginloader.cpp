#include "plugininterface.h"
#include "pluginloader.h"
#include "plugins.h"

#include <QDebug>
#include <QDir>
#include <QQmlComponent>
#include <QQmlEngine>

PluginLoader::PluginLoader(QObject *parent)
    : QObject (parent)
{
}

void PluginLoader::loadPlugin(Plugin *plugin)
{
    static bool areQmlTypesRegistered = false;
    if (!areQmlTypesRegistered) {
        registerQmlTypes();
        areQmlTypesRegistered = true;
    }

    QQmlEngine *qmlEngine = new QQmlEngine(this);
    QQmlComponent *qmlComponent = new QQmlComponent(qmlEngine, QDir(plugin->path).filePath(plugin->entryFile));
    PluginInterface *pluginInterface = qobject_cast<PluginInterface *>(qmlComponent->create(qmlComponent->creationContext()));
    if (!pluginInterface) {
        qWarning() << qmlComponent->errorString();
    }

    plugin->interface = pluginInterface;
}

void PluginLoader::registerQmlTypes()
{
    const char *url = "io.crusta.extension";
    const int majorVersion = 1;
    const int minorVersion = 0;
    qmlRegisterType<PluginInterface>(url, majorVersion, minorVersion, "PluginInterface");
}
