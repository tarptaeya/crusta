#include "theme.h"
#include "utils.h"
#include "mac/mac_utils.h"

#include <QApplication>
#include <QDir>

QStringList Theme::themeNames()
{
    QDir themeDir(QStringLiteral(":/themes"));
    QStringList list = themeDir.entryList();

    int length = list.length();
    for (int i = 0; i < length; i++) {
        list[i] = list[i].remove(QStringLiteral(".qss"));
    }

    list.removeOne(QStringLiteral("base"));

    return list;
}

void Theme::setThemeName(const QString &themeName)
{
    const QString baseThemePath = QStringLiteral(":/themes/base.qss");
    const QString themePath = QStringLiteral(":/themes/%1.qss").arg(themeName);

    const QString baseContents = Utils::readFile(baseThemePath);
    QString themeContents = Utils::readFile(themePath);

    themeContents = themeContents.prepend(baseContents);

#ifdef __APPLE__
    const QString accentColor = MacUtils::getAccentColor();
    themeContents.append(QStringLiteral("QPushButton#default {"
                                        "color: white;"
                                        "background: %1;"
                                        "}").arg(accentColor));
#endif

    qApp->setStyleSheet(themeContents);
}
