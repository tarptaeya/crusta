#ifndef CRUSTA_MAC_UTILS_H
#define CRUSTA_MAC_UTILS_H

#include "src/theme.h"
#include <QString>

class MacUtils
{
public:
    static void removeTitleBar(unsigned long long windowId);
    static void setMovableByBackground(unsigned long long windowId);

    static Color getAccentColor();
    static void setWindowBackground(unsigned long long windowId, Color color);


private:
    static void *getWindow(unsigned long long windowId);
};

#endif
