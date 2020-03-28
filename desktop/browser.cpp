#include "adblock.h"
#include "bookmarks.h"
#include "browser.h"
#include "browser_window.h"
#include "browser_schemes.h"
#include "history.h"
#include "plugins.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QIcon>
#include <QSettings>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QTextStream>
#include <QWebEngineUrlScheme>
#include <QWebEngineCookieStore>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QWebEngineSettings>
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

void Browser::load_settings()
{
    QSettings settings;

#define LOAD_WEB_ATTRIBUTE(attribute, name) \
    m_web_profile->settings()->setAttribute(QWebEngineSettings::attribute, \
    settings.value("websettings/" #name, m_web_profile->settings()->testAttribute(QWebEngineSettings::attribute)).toBool()); \

    LOAD_WEB_ATTRIBUTE(AutoLoadImages, auto_load_images)
    LOAD_WEB_ATTRIBUTE(JavascriptEnabled, javascript_enabled)
    LOAD_WEB_ATTRIBUTE(JavascriptCanOpenWindows, javascript_can_open_windows)
    LOAD_WEB_ATTRIBUTE(JavascriptCanAccessClipboard, javascript_can_access_clipboard)
    LOAD_WEB_ATTRIBUTE(LinksIncludedInFocusChain, links_included_in_focus_chain)
    LOAD_WEB_ATTRIBUTE(LocalStorageEnabled, local_storage_enabled)
    LOAD_WEB_ATTRIBUTE(LocalContentCanAccessRemoteUrls, local_content_can_access_remote_urls)
    LOAD_WEB_ATTRIBUTE(XSSAuditingEnabled, xss_auditing_enabled)
    LOAD_WEB_ATTRIBUTE(SpatialNavigationEnabled, spatial_navigation_enabled)
    LOAD_WEB_ATTRIBUTE(LocalContentCanAccessFileUrls, local_content_can_access_file_urls)
    LOAD_WEB_ATTRIBUTE(HyperlinkAuditingEnabled, hyperlink_auditing_enabled)
    LOAD_WEB_ATTRIBUTE(ScrollAnimatorEnabled, scroll_animator_enabled)
    LOAD_WEB_ATTRIBUTE(ErrorPageEnabled, error_page_enabled);
    LOAD_WEB_ATTRIBUTE(PluginsEnabled, plugins_enabled)
    LOAD_WEB_ATTRIBUTE(FullScreenSupportEnabled, fullscreen_support_enabled);
    LOAD_WEB_ATTRIBUTE(ScreenCaptureEnabled, screen_capture_enabled);
    LOAD_WEB_ATTRIBUTE(WebGLEnabled, webgl_enabled);
    LOAD_WEB_ATTRIBUTE(Accelerated2dCanvasEnabled, accelerated_2d_canvas_enabled)
    LOAD_WEB_ATTRIBUTE(AutoLoadIconsForPage, auto_load_icons_for_page)
    LOAD_WEB_ATTRIBUTE(TouchIconsEnabled, touch_icons_enabled)
    LOAD_WEB_ATTRIBUTE(FocusOnNavigationEnabled, focus_on_navigation_enabled)
    LOAD_WEB_ATTRIBUTE(PrintElementBackgrounds, print_element_backgrounds)
    LOAD_WEB_ATTRIBUTE(AllowRunningInsecureContent, allow_running_insecure_content)
    LOAD_WEB_ATTRIBUTE(AllowGeolocationOnInsecureOrigins, allow_geolocation_on_insecure_origin)
    LOAD_WEB_ATTRIBUTE(AllowWindowActivationFromJavaScript, allow_window_activation_from_javascript)
    LOAD_WEB_ATTRIBUTE(ShowScrollBars, show_scroll_bars)
    LOAD_WEB_ATTRIBUTE(PlaybackRequiresUserGesture, playback_requires_user_gesture)
    LOAD_WEB_ATTRIBUTE(JavascriptCanPaste, javascript_can_paste)
    LOAD_WEB_ATTRIBUTE(WebRTCPublicInterfacesOnly, webrtc_public_interfaces_only)
    LOAD_WEB_ATTRIBUTE(DnsPrefetchEnabled, dns_prefetch_enabled)
    LOAD_WEB_ATTRIBUTE(PdfViewerEnabled, pdf_viewer_enabled)

#undef LOAD_WEB_ATTRIBUTE
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

    QCommandLineParser parser;
    QCommandLineOption private_mode_option(QStringList() << QStringLiteral("p") << QStringLiteral("private"));
    parser.addOption(private_mode_option);
    parser.process(app);

    m_is_private = parser.isSet(private_mode_option);

    setup_web_profile();
    setup_database();

    m_adblock = new Adblock;
    m_history_model = new HistoryModel;
    m_bookmark_model = new BookmarkModel;
    m_plugins = new Plugins;

    load_settings();
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

bool Browser::is_private() const
{
    return m_is_private;
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
