#pragma once

#include <QObject>

#define appManager AppManager::instance()

class Window;
class Settings;

class AppManager : public QObject
{
    Q_OBJECT
    enum ApplicationMode {
        Development,
        Alpha,
        Beta,
        Release
    };
    struct Application {
        QString organisationName;
        QString organisationDomain;
        QString name;
        int majorVersion;
        int minorVersion;
        int patchVersion;
        ApplicationMode mode;
    };
public:
    explicit AppManager(QObject *parent = nullptr);
    ~AppManager();
    static AppManager *instance();

    QString organisationName();
    QString organisationDomain();
    QString applicationName();
    int majorVersion();
    int minorVersion();
    int patchVersion();
    QString applicationVersionString();

    void bootstrap();
    Settings *settings();

private:
    Application m_application;
    QList<Window *> m_windowList;
    Settings *m_settings = nullptr;
};
