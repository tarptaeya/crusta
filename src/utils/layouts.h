#pragma once

class Layouts
{
    explicit Layouts();
public:
    enum Position {
        Left,
        Right,
        Top,
        Bottom
    };
    static Position sideBarPosition();
};
