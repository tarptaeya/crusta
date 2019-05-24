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
    const Color accentColor = MacUtils::getAccentColor();
    themeContents.append(QStringLiteral("QPushButton#default {"
                                        "color: white;"
                                        "background: rgb(%1, %2, %3);"
                                        "}").arg(accentColor.red).arg(accentColor.green).arg(accentColor.blue));
#endif

    qApp->setStyleSheet(themeContents);
}

void Theme::appendColorToTheme(Color color)
{
#ifdef __APPLE__
    QString theme = qApp->styleSheet();
    theme = theme.append(QStringLiteral("#sidebar {"
                                        "background: rgb(%1, %2, %3);"
                                        "}"
                                        ""
                                        "QSplitter:handle {"
                                        "background: rgb(%1, %2, %3);"
                                        "}"
                                        ""
                                        "QToolBar {"
                                        "background: rgb(%1, %2, %3);"
                                        "}").arg(color.red).arg(color.green).arg(color.blue));
    qApp->setStyleSheet(theme);
#endif
}
