#pragma once

#include <QString>
#include <QStringList>

struct Color
{
    int red;
    int green;
    int blue;
};

class Theme
{
public:
    static QStringList themeNames();
    static void setThemeName(const QString &themeName);

    static void appendColorToTheme(Color color);
};
