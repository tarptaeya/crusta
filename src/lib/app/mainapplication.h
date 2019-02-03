#ifndef CR_MAIN_APPLICATION_H
#define CR_MAIN_APPLICATION_H

#include <QObject>
#include <QSettings>
#include <QWebEngineProfile>

#define appManager MainApplication::instance()
#define CRUSTA_VERSION_STR QSL("2.0.0")

class BrowserWindow;
class DataBase;
class Manager;
class Plugins;
class Preferences;
class Tab;
class WebDialogWindow;
class WebView;

class MainApplication : public QObject
{
public:
    explicit MainApplication(bool isPrivate = false, QObject *parent = nullptr);
    ~MainApplication();
    int exec();
    static MainApplication *instance(bool isPrivate = false);
    bool isPrivate();

    BrowserWindow *createWindow();
    BrowserWindow *createWindow(Tab *tab);
    void removeWindow();
    QList<BrowserWindow *> windows();
    BrowserWindow *currentWindow();

    void createDialogWindow(WebView *webView);

    DataBase *dataBase();
    Manager *manager();
    Preferences *preferences();
    QSettings *settings();

    void initWebEngineProfile();
    QWebEngineProfile *webEngineProfile();

    void loadPlugins();
    Plugins *plugins();

    void aboutCrusta();
    QString defaultUserAgent() const;

private:
    bool m_isPrivate = false;

    QList<BrowserWindow *> m_windows;
    DataBase *m_dataBase = nullptr;
    Manager *m_manager = nullptr;
    Preferences *m_preferences = nullptr;
    QSettings *m_settings = nullptr;

    QWebEngineProfile *m_webEngineProfile = nullptr;
    Plugins *m_plugins = nullptr;
};

#endif
