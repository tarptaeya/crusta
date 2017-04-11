#ifndef PRESENTATIONMODENOTIFIER_H
#define PRESENTATIONMODENOTIFIER_H

#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QWebEngineView>
#include <QApplication>
#include <QDesktopWidget>



class PresentationModeNotifier:public QObject{
public:
    QLabel* label=new QLabel();
    QTimer* timer=new QTimer();
    int x;
    void createNotifier();
    void showNotifier();
    void fadeOut();
    void setViewParent(QWebEngineView* view);
    void hideNotifier();
};



#endif // PRESENTATIONMODENOTIFIER_H
