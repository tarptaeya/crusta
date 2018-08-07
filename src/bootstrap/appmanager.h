#pragma once

#include <QObject>

class Window;

class AppManager : public QObject
{
    Q_OBJECT
public:
    explicit AppManager(QObject *parent = nullptr);
    ~AppManager();

    void bootstrap();

private:
    QList<Window *> m_windowList;
};
