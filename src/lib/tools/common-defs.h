#ifndef CR_COMMON_DEFS_H
#define CR_COMMON_DEFS_H

#include <QString>

#define QSL QStringLiteral
#define QL1C QLatin1Char
#define QL1S QLatin1String

QString readFile(const QString &path);

#endif
