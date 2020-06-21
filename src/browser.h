#ifndef BROWSER_H
#define BROWSER_H

#include <QObject>
#include <QVector>

class Window;

#define crusta Browser::instance()

class Browser : public QObject
{
    Q_OBJECT
    QVector<Window *> m_windows;
public:
    Browser(QObject *parent = nullptr);

    static Browser *instance();
    void run();

    Window *createWindow();
};

#endif // BROWSER_H
