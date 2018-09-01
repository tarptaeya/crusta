#pragma once

#include <QMargins>

class Dimensions
{
    explicit Dimensions();
public:
    static int sideBarWidth();
    static int tabBarHeight();
    static int tabListWidth();
    static int tabListItemHeight();
    static int onmibarHeightOffsetFromParent();
    static QMargins lineEditMargins();
    static int contextMenuTextLength();
};
