#include "adblock.h"
#include "bookmarks.h"
#include "browser.h"
#include "browser_window.h"
#include "browser_schemes.h"
#include "history.h"
#include "plugins.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QTextStream>
#include <QWebEngineUrlScheme>
#include <QWebEngineCookieStore>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QWidget>

void Browser::setup_web_profile()
{
    m_web_profile = QWebEngineProfile::defaultProfile();

    BrowserSchemeHandler *browser_scheme_handler = new BrowserSchemeHandler(m_web_profile);
    m_web_profile->installUrlSchemeHandler("browser", browser_scheme_handler);

    m_web_profile->cookieStore()->setCookieFilter([](const QWebEngineCookieStore::FilterRequest &request) {
        return !request.thirdParty;
    });

    QDir scriptsDir(QStringLiteral(":assets/scripts/"));
    const QStringList scripts = scriptsDir.entryList();
    for (const QString &path : scripts) {
        QFile file(QStringLiteral(":assets/scripts/%1").arg(path));
        if (!file.open(QFile::ReadOnly))
            continue;

        QTextStream stream(&file);
        const QString source = stream.readAll();

        QWebEngineScript script;
        script.setName(QStringLiteral("%1").arg(path));
        script.setWorldId(QWebEngineScript::ApplicationWorld);
        script.setSourceCode(source);
        script.setInjectionPoint(QWebEngineScript::DocumentReady);
        script.setRunsOnSubFrames(false);
        m_web_profile->scripts()->insert(script);
    }
}

void Browser::setup_database()
{
    const QString driver = QStringLiteral("QSQLITE");
    if (!QSqlDatabase::isDriverAvailable(driver)) {
        qDebug() << "Driver not available";
        return;
    }

    m_database = QSqlDatabase::addDatabase(driver);
    if (m_is_private) {
        m_database.setDatabaseName(QStringLiteral(":memory:"));
    } else {
        QDir standardLocation(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        const QString dbPath = standardLocation.absoluteFilePath(QStringLiteral("database"));
        m_database.setDatabaseName(dbPath);
    }

    m_database.open();
    if (!m_database.isOpen()) {
        qDebug() << m_database.lastError();
        return;
    }

    QSqlQuery query;
    query.prepare(QStringLiteral("CREATE TABLE IF NOT EXISTS history (title TEXT, address TEXT UNIQUE, icon BLOB, last_visited DATETIME)"));
    if (!query.exec()) {
        qDebug() << query.lastError();
    }
}

Browser::~Browser()
{
    if (m_database.isOpen())
        m_database.close();

    delete m_adblock;
    delete m_history_model;
    delete m_bookmark_model;
    delete m_plugins;
}

int Browser::start(int argc, char **argv)
{
    QApplication::setOrganizationName(QStringLiteral("Crusta"));
    QApplication::setOrganizationDomain(QStringLiteral("desktop.crustabrowser.com"));
    QApplication::setApplicationName(QStringLiteral("Crusta"));
    QApplication::setApplicationVersion(QStringLiteral("2.0.0"));

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QIcon::setThemeSearchPaths(QIcon::themeSearchPaths() << QStringLiteral(":assets/icons"));
    QIcon::setThemeName(QStringLiteral("breeze"));
    QApplication::setStyle(QStyleFactory::create(QStringLiteral("fusion")));

    register_scheme("browser");

    QApplication app(argc, argv);

    setup_web_profile();
    setup_database();

    m_adblock = new Adblock;
    m_history_model = new HistoryModel;
    m_bookmark_model = new BookmarkModel;
    m_plugins = new Plugins;

    create_browser_window();
    return app.exec();
}

BrowserWindow *Browser::create_browser_window() const
{
    BrowserWindow *window = new BrowserWindow;
    window->show();
    return window;
}

void Browser::register_scheme(const QByteArray &name) const
{
    QWebEngineUrlScheme scheme(name);
    scheme.setFlags(scheme.flags() | QWebEngineUrlScheme::LocalScheme | QWebEngineUrlScheme::SecureScheme);
    scheme.setSyntax(QWebEngineUrlScheme::Syntax::Host);
    QWebEngineUrlScheme::registerScheme(scheme);
}

QWebEngineProfile *Browser::web_profile() const
{
    return m_web_profile;
}

Adblock *Browser::adblock() const
{
    return m_adblock;
}

HistoryModel *Browser::history_model() const
{
    return m_history_model;
}

BookmarkModel *Browser::bookmark_model() const
{
    return m_bookmark_model;
}

Plugins *Browser::plugins() const
{
    return m_plugins;
}

Browser *Browser::instance()
{
    static Browser *browser_ = new Browser;
    return browser_;
}
