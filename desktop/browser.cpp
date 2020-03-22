#include "browser.h"
#include "browser_window.h"
#include "browser_schemes.h"

#include <QApplication>
#include <QIcon>
#include <QStyleFactory>
#include <QWebEngineUrlScheme>
#include <QWidget>

#include <QDebug>
void Browser::setup_web_profile()
{
    m_web_profile = QWebEngineProfile::defaultProfile();

    BrowserSchemeHandler *browser_scheme_handler = new BrowserSchemeHandler(m_web_profile);
    m_web_profile->installUrlSchemeHandler("browser", browser_scheme_handler);
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
