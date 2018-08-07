#include "strings.h"

Strings::Strings()
{
}

Strings::~Strings()
{
}

QString Strings::windowName()
{
    return QStringLiteral("window");
}

QString Strings::sideBarName()
{
    return QStringLiteral("sideBar");
}
