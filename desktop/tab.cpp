#include "bookmarks.h"
#include "browser.h"
#include "history.h"
#include "tab.h"
#include "webview.h"

#include <QIcon>
#include <QRegularExpression>
#include <QVBoxLayout>
#include <QWebEngineHistory>

Tab::Tab(QWidget *parent)
    : QWidget(parent)
{
}

void WebTab::setup_toolbar()
{
    auto create_tool_button = [](const QString &name) {
        QToolButton *button = new QToolButton;
        button->setAutoRaise(true);
        button->setIcon(QIcon::fromTheme(name));
        return button;
    };

    m_back_button = create_tool_button(QStringLiteral("go-previous"));
    m_forward_button = create_tool_button(QStringLiteral("go-next"));
    m_refresh_button = create_tool_button(QStringLiteral("view-refresh"));
    m_home_button = create_tool_button(QStringLiteral("go-home"));

    m_address_bar = new QLineEdit;
    m_bookmark_action = m_address_bar->addAction(QIcon::fromTheme(QStringLiteral("bookmark-new")), QLineEdit::TrailingPosition);

    m_toolbar->addWidget(m_back_button);
    m_toolbar->addWidget(m_forward_button);
    m_toolbar->addWidget(m_refresh_button);
    m_toolbar->addWidget(m_home_button);
    m_toolbar->addWidget(m_address_bar);

    connect(m_back_button, &QToolButton::clicked, m_webview, &WebView::back);
    connect(m_forward_button, &QToolButton::clicked, m_webview, &WebView::forward);
    connect(m_refresh_button, &QToolButton::clicked, [this] {
        const QString icon_name = m_refresh_button->icon().name();
        if (icon_name == QStringLiteral("view-refresh")) {
            m_webview->reload();
        } else {
            m_webview->stop();
        }
    });
    connect(m_home_button, &QToolButton::clicked, m_webview, &WebView::home);
    connect(m_address_bar, &QLineEdit::returnPressed, [this] {
        const QString text = m_address_bar->text();
        if (QUrl::fromUserInput(text).isValid()) {
            m_webview->load(text);
        } else {
            // TODO: search with default search engine
        }

        m_webview->setFocus();
    });

    connect(m_bookmark_action, &QAction::triggered, [this] {
        BookmarkTreeNode *node = new BookmarkTreeNode(BookmarkTreeNode::Address);
        node->title = m_webview->title();
        node->address = m_webview->url().toString();
        browser->bookmark_model()->add_bookmark(nullptr, node);
    });

    connect(m_webview, &WebView::urlChanged, [this] (const QUrl &address) {
        m_address_bar->setText(address.toDisplayString());
        m_address_bar->setCursorPosition(0);
    });
    connect(m_webview, &WebView::loadStarted, [this] {
        m_refresh_button->setIcon(QIcon::fromTheme(QStringLiteral("process-stop")));
    });
    connect(m_webview, &WebView::loadFinished, [this] {
        m_back_button->setEnabled(m_webview->history()->canGoBack());
        m_forward_button->setEnabled(m_webview->history()->canGoForward());
        m_refresh_button->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    });
}

WebTab::WebTab(QWidget *parent)
    : Tab(parent)
{
    m_toolbar = new QToolBar;
    m_webview = new WebView;

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0, 0, 0, 0);
    vbox->setSpacing(0);
    setLayout(vbox);

    vbox->addWidget(m_toolbar);
    vbox->addWidget(m_webview);

    setup_toolbar();

    connect(m_webview, &WebView::titleChanged, [this] (const QString &title) { emit title_changed(title); });
    connect(m_webview, &WebView::iconChanged, [this] (const QIcon &icon) { emit icon_changed(icon); });
}

QToolBar *WebTab::toolbar() const
{
    return m_toolbar;
}

WebView *WebTab::webview() const
{
    return m_webview;
}

void ManagerTab::setup_toolbar()
{
    m_toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QAction *history = m_toolbar->addAction(QIcon::fromTheme(QStringLiteral("appointment-new")), QStringLiteral("History"));
    QAction *bookmarks = m_toolbar->addAction(QIcon::fromTheme(QStringLiteral("bookmark-new")), QStringLiteral("Bookmarks"));

    connect(history, &QAction::triggered, [this] { open_history(); });
    connect(bookmarks, &QAction::triggered, [this] { open_bookmarks(); });
}

void ManagerTab::setup_stacked_widget()
{
    HistoryWidget *history_widget = new HistoryWidget;
    m_stacked_widget->addWidget(history_widget);

    BookmarkWidget *bookmark_widget = new BookmarkWidget;
    int index = m_stacked_widget->addWidget(bookmark_widget);

    m_stacked_widget->setCurrentIndex(index);
}

ManagerTab::ManagerTab(QWidget *parent)
    : Tab(parent)
{
    m_toolbar = new QToolBar;
    m_stacked_widget = new QStackedWidget;

    QVBoxLayout *vbox = new QVBoxLayout;
    setLayout(vbox);

    vbox->addWidget(m_toolbar);
    vbox->addWidget(m_stacked_widget);

    setup_toolbar();
    setup_stacked_widget();
}

void ManagerTab::open_history()
{
    m_stacked_widget->setCurrentIndex(0);
    emit title_changed(QStringLiteral("History"));
    emit icon_changed(QIcon::fromTheme(QStringLiteral("appointment-new")));
}

void ManagerTab::open_bookmarks()
{
    m_stacked_widget->setCurrentIndex(1);
    emit title_changed(QStringLiteral("Bookmarks"));
    emit icon_changed(QIcon::fromTheme(QStringLiteral("bookmark-new")));
}
