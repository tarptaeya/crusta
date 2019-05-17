#pragma once

#include "plugin.h"

#include <QObject>

class Plugins : public QObject
{
public:
    explicit Plugins(QObject *parent = nullptr);
    ~Plugins();

private:
    QList<Plugin *> m_plugins;

    void installTypes();
    void loadPlugin(const QString &path);
    void loadAllPlugins();
    void unloadAllPlugins();
};
