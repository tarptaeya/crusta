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

QString MacUtils::getAccentColor()
{
    NSColor *accentColor;
    if (@available(macOS 10.14, *)) {
        accentColor = [NSColor controlAccentColor];
    } else {
        accentColor = [NSColor systemBlueColor];
    }

    accentColor = [accentColor colorUsingColorSpace:NSColorSpace.genericRGBColorSpace];
    int redComponent = static_cast<int>(255 * [accentColor redComponent]);
    int greenComponent = static_cast<int>(255 * [accentColor greenComponent]);
    int blueComponent = static_cast<int>(255 * [accentColor blueComponent]);

    return QStringLiteral("rgb(%1, %2, %3)").arg(redComponent).arg(greenComponent).arg(blueComponent);
}

void *MacUtils::getWindow(unsigned long long windowId)
{
    NSView *view = reinterpret_cast<NSView *>(windowId);
    return [view window];
}
