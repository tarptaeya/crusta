#ifndef CR_PLUGIN_LOADER_H
#define CR_PLUGIN_LOADER_H

#include <QObject>

struct Plugin;
class PluginInterface;

class PluginLoader : public QObject
{
public:
    explicit PluginLoader(QObject *parent = nullptr);
    void loadPlugin(Plugin *plugin);

private:
    static void registerQmlTypes();
};

#endif
