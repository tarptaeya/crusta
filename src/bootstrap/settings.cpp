#include "settings.h"

Settings::Settings(QObject *parent)
{
    QSettings::setParent(parent);
}
