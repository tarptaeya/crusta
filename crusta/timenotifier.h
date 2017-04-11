#ifndef TIMENOTIFIER_H
#define TIMENOTIFIER_H

#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QWebEngineView>

class TimeNotifier:public QObject{
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


#endif // TIMENOTIFIER_H
