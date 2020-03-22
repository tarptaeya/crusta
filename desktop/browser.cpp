#include "browser.h"
#include "browser_window.h"
#include "browser_schemes.h"
#include "preferences.h"

#include <QApplication>
#include <QDir>
#include <QIcon>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QWebEngineUrlScheme>
#include <QWidget>

#include <iostream>

void Browser::setup_database()
{
    const QString driver(QStringLiteral("QSQLITE"));
    if (!QSqlDatabase::isDriverAvailable(driver)) {
        std::cerr << "sqlite driver not available" << std::endl;
        return;
    }

    m_database = QSqlDatabase::addDatabase(driver);
    if (m_is_private) {
        m_database.setDatabaseName(QStringLiteral(""));
    } else {
        QDir standardLocation(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
        const QString dbPath = standardLocation.absoluteFilePath(QStringLiteral("database"));
        m_database.setDatabaseName(dbPath);
    }

    if (!m_database.open()) {
        std::cerr << "unable to open database: " << m_database.lastError().text().toStdString() << std::endl;
        return;
    }

    QStringList query_list;
    query_list << QStringLiteral("CREATE TABLE IF NOT EXISTS history (title TEXT, address TEXT UNIQUE, icon BLOB, icon_url TEXT, last_visited DATETIME DEFAULT current_timestamp, visit_count INTEGER DEFAULT 1)");

    QSqlQuery query;
    for (const QString &query_string : qAsConst(query_list)) {
        if (!query.exec(query_string)) {
            std::cerr << "unable to execute query: " <<  query.lastQuery().toStdString() << " " << query.lastError().text().toStdString() << std::endl;
        }
    }
}

void Browser::setup_preferences_window()
{
    m_preferences_window = new PreferencesWindow;
}

void Browser::setup_web_profile()
{
    m_web_profile = QWebEngineProfile::defaultProfile();

    BrowserSchemeHandler *browser_scheme_handler = new BrowserSchemeHandler(m_web_profile);
    m_web_profile->installUrlSchemeHandler("browser", browser_scheme_handler);
}

Browser::~Browser()
{
    if (m_database.isOpen())
        m_database.close();

    delete m_preferences_window;
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
    setup_database();
    setup_preferences_window();
    setup_web_profile();
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

Browser *Browser::instance()
{
    static Browser *browser_ = new Browser;
    return browser_;
}

void Browser::show_preferences_window()
{
    m_preferences_window->show();
}
