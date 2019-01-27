#ifndef CR_PLUGINS_H
#define CR_PLUGINS_H

#include <QList>
#include <QObject>
#include <QString>

class PluginInterface;

struct Plugin
{
    QString name;
    QString description;
    QString entryFile;
    QString iconFile;
    QString author;
    QString email;
    QString path;

    bool enabled = false;

    PluginInterface *interface = nullptr;
};

class Plugins : public QObject
{
public:
    explicit Plugins(QObject *parent = nullptr);
    void loadPlugins();

    QList<Plugin *> plugins();
private:
    void loadInternalPlugins();
    void loadExternalPlugins();
    void loadPluginsFromPath(const QString &path);

    void loadPlugin(Plugin *plugin);
    Plugin *parseMetaData(const QString &path);

    QList<Plugin *> m_plugins;
};

#endif
