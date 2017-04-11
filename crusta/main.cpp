#include "mainview.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    a.setApplicationName(QString("Crusta"));
    a.setApplicationVersion(QString("1.0"));
    Q_INIT_RESOURCE(resource); // initialised the resource file;
    MainView* w=new MainView();
    w->showView();

    return a.exec();
}
