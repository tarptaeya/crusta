#ifndef BROWSER_H
#define BROWSER_H

#include <QObject>
#include <QVector>
#include <QWebEngineProfile>

class Window;

#define crusta Browser::instance()

class Browser : public QObject
{
    Q_OBJECT
    QVector<Window *> m_windows;
    QWebEngineProfile *m_profile = nullptr;

    void setupProfile();
public:
    Browser(QObject *parent = nullptr);

    static Browser *instance();
    void run();

    Window *createWindow();
    QWebEngineProfile *profile() const;
};

#endif // BROWSER_H
