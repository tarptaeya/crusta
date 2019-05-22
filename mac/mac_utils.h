#ifndef CRUSTA_MAC_UTILS_H
#define CRUSTA_MAC_UTILS_H

#include <QString>

class MacUtils
{
public:
    static void removeTitleBar(unsigned long long windowId);
    static void setMovableByBackground(unsigned long long windowId);

    static QString getAccentColor();

private:
    static void *getWindow(unsigned long long windowId);
};

#endif
