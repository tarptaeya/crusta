#ifndef FULLSCREENNOTIFIER_H
#define FULLSCREENNOTIFIER_H

#include <QObject>
#include <QLabel>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QWebEngineView>

class FullScreenNotifier:public QObject{
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

#endif // FULLSCREENNOTIFIER_H
