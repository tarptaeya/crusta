#ifndef CR_SCRIPTS_H
#define CR_SCRIPTS_H

#include <QString>

struct Script
{
    QString name;
    QString source;
};

class Scripts
{
public:
    static QString webChannelScript();
    static QList<Script> loadInternalScripts();
    static QList<Script> loadScriptsFromPath(const QString &path);
};

#endif
