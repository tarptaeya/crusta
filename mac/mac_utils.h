#ifndef CRUSTA_MAC_UTILS_H
#define CRUSTA_MAC_UTILS_H

class MacUtils
{
public:
    static void removeTitleBar(unsigned long long windowId);
    static void setMovableByBackground(unsigned long long windowId);

private:
    static void *getWindow(unsigned long long windowId);
};

#endif
