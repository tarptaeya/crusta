#import "mac_utils.h"

#import <Cocoa/Cocoa.h>
#include <QMainWindow>

void MacUtils::removeTitleBar(unsigned long long windowId)
{
    NSWindow *window = reinterpret_cast<NSWindow *>(getWindow(windowId));
    if (!window) {
        return;
    }

    window.titlebarAppearsTransparent = YES;
}

void MacUtils::setMovableByBackground(unsigned long long windowId)
{
    NSWindow *window = reinterpret_cast<NSWindow *>(getWindow(windowId));
    if (!window) {
        return;
    }

    [window setMovableByWindowBackground:YES];
}

Color MacUtils::getAccentColor()
{
    NSColor *accentColor;
    if (@available(macOS 10.14, *)) {
        accentColor = [NSColor controlAccentColor];
    } else {
        accentColor = [NSColor systemBlueColor];
    }

    accentColor = [accentColor colorUsingColorSpace:NSColorSpace.genericRGBColorSpace];

    Color color;
    color.red = static_cast<int>(255 * [accentColor redComponent]);
    color.green = static_cast<int>(255 * [accentColor greenComponent]);
    color.blue = static_cast<int>(255 * [accentColor blueComponent]);

    return color;
}

void MacUtils::setWindowBackground(unsigned long long windowId, Color color)
{
    NSWindow *window = reinterpret_cast<NSWindow *>(getWindow(windowId));
    if (!window) {
        return;
    }

    CGFloat red = static_cast<CGFloat>(color.red / 255.0f);
    CGFloat green = static_cast<CGFloat>(color.green / 255.0f);
    CGFloat blue = static_cast<CGFloat>(color.blue / 255.0f);
    NSColor *nsColor = [NSColor colorWithRed:red green:green blue:blue alpha:1.0];
    window.backgroundColor = nsColor;
}

void *MacUtils::getWindow(unsigned long long windowId)
{
    NSView *view = reinterpret_cast<NSView *>(windowId);
    return [view window];
}
