#include "common-defs.h"
#include "scripts.h"

#include <QDir>
#include <QStringList>

QString Scripts::webChannelScript()
{
    return readFile(QSL(":qtwebchannel/qwebchannel.js"));
}

QList<Script> Scripts::loadInternalScripts()
{
    return loadScriptsFromPath(QSL(":/scripts"));
}

QList<Script> Scripts::loadScriptsFromPath(const QString &path)
{
    QList<Script> scripts;
    QDir dir(path);
    const QStringList entries = dir.entryList();
    for (const QString &entry : qAsConst(entries)) {
        Script script;
        script.name = entry;
        script.source = readFile(dir.filePath(entry));
        scripts.append(script);
    }
    return scripts;
}
