#pragma once

#include "plugin.h"

#include <QObject>

class Plugins : public QObject
{
public:
    explicit Plugins(QObject *parent = nullptr);
    ~Plugins();

    void loadPlugin(const QString &path);
    QList<Plugin *> plugins();

private:
    QList<Plugin *> m_plugins;

    void installTypes();
    void loadAllPlugins();
    void unloadAllPlugins();
};
