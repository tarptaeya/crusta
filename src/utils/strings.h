#pragma once

#include <QString>

class Strings
{
public:
    explicit Strings();
    ~Strings();

    static QString windowName();
    static QString sideBarName();
    static QString elideString(QString source, int length);
};
