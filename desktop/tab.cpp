#include "tab.h"
#include "webview.h"

#include <QRegularExpression>
#include <QVBoxLayout>
#include <QWebEngineHistory>

void Tab::setup_toolbar()
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

Tab::Tab(QWidget *parent)
    : QWidget(parent)
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

WebView *Tab::webview() const
{
    return m_webview;
}
