#include "common-defs.h"
#include "mainapplication.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QCommandLineParser parser;
    QCommandLineOption privateModeOption(QStringList() << QSL("p") << QSL("private"));
    parser.addOption(privateModeOption);

    parser.process(app);

    MainApplication::instance(parser.isSet(privateModeOption))->createWindow();
    int retCode = app.exec();

    delete MainApplication::instance();

    return retCode;
}
