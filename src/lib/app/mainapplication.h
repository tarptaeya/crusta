#ifndef CR_MAIN_APPLICATION_H
#define CR_MAIN_APPLICATION_H

#include <QObject>
#include <QSettings>
#include <QWebEngineProfile>

#define appManager MainApplication::instance()
#define CRUSTA_VERSION_STR QSL("2.0.0")

class BrowserWindow;
class DataBase;
class Plugins;

class MainApplication : public QObject
{
public:
    explicit MainApplication(QObject *parent = nullptr);
    int exec();
    static MainApplication *instance();

    BrowserWindow *createWindow();
    BrowserWindow *createWindow(const QString &startPageUrl);
    void removeWindow();
    QList<BrowserWindow *> windows();
    BrowserWindow *currentWindow();

    DataBase *dataBase();
    QSettings *settings();

    void initWebEngineProfile();
    QWebEngineProfile *webEngineProfile();

    void loadPlugins();
    Plugins *plugins();

    void aboutCrusta();

private:
    QList<BrowserWindow *> m_windows;
    DataBase *m_dataBase = nullptr;
    QSettings *m_settings = nullptr;

    QWebEngineProfile *m_webEngineProfile = nullptr;
    Plugins *m_plugins = nullptr;
};

#endif
