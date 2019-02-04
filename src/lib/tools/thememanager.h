#ifndef CR_THEME_MANAGER_H
#define CR_THEME_MANAGER_H

#include <QString>

class ThemeManager
{
public:
    explicit ThemeManager();
    static QString defaultTheme();

    static void clearCurrentTheme();

    static QStringList getThemeNames();
    static void loadThemeFromPath(const QString &path);
};

#endif
