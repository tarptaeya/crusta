#include "common-defs.h"
#include "standardpaths.h"

#include <QDir>

QString StandardPaths::settingsPath()
{
    return QDir::homePath() + QDir::separator() + QSL("./crusta/settings.ini");
}
