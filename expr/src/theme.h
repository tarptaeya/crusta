#pragma once

#include <QString>
#include <QStringList>

class Theme
{
public:
    static QStringList themeNames();
    static void setThemeName(const QString &themeName);
};
