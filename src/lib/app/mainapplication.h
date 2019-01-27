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
class Tab;
class WebDialogWindow;
class WebView;

class MainApplication : public QObject
{
public:
    explicit MainApplication(QObject *parent = nullptr);
    ~MainApplication();
    int exec();
    static MainApplication *instance();

    BrowserWindow *createWindow();
    BrowserWindow *createWindow(Tab *tab);
    void removeWindow();
    QList<BrowserWindow *> windows();
    BrowserWindow *currentWindow();

    void createDialogWindow(WebView *webView);

    DataBase *dataBase();
    Manager *manager();
    QSettings *settings();

    void initWebEngineProfile();
    QWebEngineProfile *webEngineProfile();

    void loadPlugins();
    Plugins *plugins();

    void aboutCrusta();

private:
    QList<BrowserWindow *> m_windows;
    DataBase *m_dataBase = nullptr;
    Manager *m_manager = nullptr;
    QSettings *m_settings = nullptr;

    QWebEngineProfile *m_webEngineProfile = nullptr;
    Plugins *m_plugins = nullptr;
};

#endif
