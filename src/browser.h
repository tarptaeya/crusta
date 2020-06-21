#ifndef BROWSER_H
#define BROWSER_H

#include <QObject>

#define crusta Browser::instance()

class Browser : public QObject
{
public:
    Browser(QObject *parent = nullptr);

    static Browser *instance();

    void run();
};

#endif // BROWSER_H
