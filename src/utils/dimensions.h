#pragma once

class Dimensions
{
    explicit Dimensions();
public:
    static int sideBarWidth();
    static int tabBarHeight();
private:
    static int DEFAULT_SIDEBAR_WIDTH;
    static int DEFAULT_TABBAR_HEIGHT;
};
