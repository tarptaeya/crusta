#import "mac_utils.h"

#import <Cocoa/Cocoa.h>

void MacUtils::removeTitleBar(unsigned long long windowId)
{
    NSWindow *window = reinterpret_cast<NSWindow *>(getWindow(windowId));
    if (!window) {
        return;
    }

    [window setTitlebarAppearsTransparent:YES];
}

void MacUtils::setMovableByBackground(unsigned long long windowId)
{
    NSWindow *window = reinterpret_cast<NSWindow *>(getWindow(windowId));
    if (!window) {
        return;
    }

    [window setMovableByWindowBackground:YES];
}

void *MacUtils::getWindow(unsigned long long windowId)
{
    NSView *view = reinterpret_cast<NSView *>(windowId);
    return [view window];
}
