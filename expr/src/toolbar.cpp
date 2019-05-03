#include "toolbar.h"

#include <QMenu>
#include <QUrl>

ToolBar::ToolBar(QWidget *parent)
    : QToolBar (parent)
{
    m_backButton = new QToolButton;
    m_forwardButton = new QToolButton;
    m_refreshButton = new QToolButton;
    m_homeButton = new QToolButton;
    m_bookmarkButton = new QToolButton;
    m_menuButton = new QToolButton;
    m_addressBar = new QLineEdit;
    m_backMenu = new QMenu;
    m_forwardMenu = new QMenu;

    m_backButton->setIcon(QIcon::fromTheme(QStringLiteral("go-previous")));
    m_forwardButton->setIcon(QIcon::fromTheme(QStringLiteral("go-next")));
    m_refreshButton->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
    m_homeButton->setIcon(QIcon::fromTheme(QStringLiteral("go-home")));

    m_backButton->setPopupMode(QToolButton::DelayedPopup);
    m_forwardButton->setPopupMode(QToolButton::DelayedPopup);

    m_backButton->setMenu(m_backMenu);
    m_forwardButton->setMenu(m_forwardMenu);

    addWidget(m_backButton);
    addWidget(m_forwardButton);
    addWidget(m_refreshButton);
    addWidget(m_homeButton);
    addWidget(m_addressBar);
    addWidget(m_bookmarkButton);
    addSeparator();
    addWidget(m_menuButton);

    setMovable(false);

    connect(m_backButton, &QToolButton::clicked, this, [this] { emit backRequested(); });
    connect(m_forwardButton, &QToolButton::clicked, this, [this] { emit forwardRequested(); });
    connect(m_refreshButton, &QToolButton::clicked, this, [this] { emit loadingStateChangeRequest(); });
}

void ToolBar::setUrl(const QUrl &url)
{
    m_addressBar->setText(url.toDisplayString());
    m_addressBar->setCursorPosition(0);
}

void ToolBar::loadStarted()
{
    m_refreshButton->setIcon(QIcon::fromTheme(QStringLiteral("process-stop")));
}

void ToolBar::loadFinished()
{
    m_refreshButton->setIcon(QIcon::fromTheme(QStringLiteral("view-refresh")));
}

void ToolBar::setHistory(QWebEngineHistory *history)
{
    m_backMenu->clear();
    m_forwardMenu->clear();

    const QList<QWebEngineHistoryItem> backItems = history->backItems(10);
    const QList<QWebEngineHistoryItem> forwardItems = history->forwardItems(10);

    for (const QWebEngineHistoryItem &item : backItems) {
        QAction *action = m_backMenu->addAction(item.title());
        connect(action, &QAction::triggered, this, [this, item] { emit navigationToItemRequest(item); });
    }

    for (const QWebEngineHistoryItem &item : forwardItems) {
        QAction *action = m_forwardMenu->addAction(item.title());
        connect(action, &QAction::triggered, this, [this, item] { emit navigationToItemRequest(item); });
    }

    m_backButton->setDisabled(!history->canGoBack());
    m_forwardButton->setDisabled(!history->canGoForward());
}
