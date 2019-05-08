#import "mac_utils.h"

#import <Cocoa/Cocoa.h>

void MacUtils::removeTitleBar(unsigned long long windowId)
{
    NSView *view = reinterpret_cast<NSView *>(windowId);
    NSWindow *window = [view window];
    [window setTitlebarAppearsTransparent:YES];
}
