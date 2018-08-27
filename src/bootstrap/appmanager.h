#pragma once

#include <QObject>
#include <QWebEngineProfile>
#include <QWebEngineScriptCollection>
#include <QWebEngineScript>

#define appManager AppManager::instance()

class Window;
class Settings;
class ExternalJsObject;
class Database;

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
    QWebEngineProfile *webEngineProfile() const;
    Database *database() const;
private:
    Application m_application;
    QList<Window *> m_windowList;
    Settings *m_settings = nullptr;
    QWebEngineProfile *m_webEngineProfile = nullptr;
    Database *m_database;

    void setUpWebEngineProfile();
};
