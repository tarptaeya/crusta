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

QString Strings::elideString(QString source, int length)
{
    QString elidedString = source.left(length);
    if (elidedString.length() < source.length()) {
        elidedString = elidedString.append(QStringLiteral("..."));
    }
    return elidedString;
}
