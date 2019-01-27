#include "bookmarksmanager.h"
#include "common-defs.h"
#include "downloadsmanager.h"
#include "historymanager.h"
#include "manager.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

Manager::Manager(QWidget *parent)
    : QWidget (parent)
{
    m_tabWidget = new QTabWidget(this);

    m_historyManager = new HistoryManager(this);
    m_bookmarksManager = new BookmarksManager(this);
    m_downloadsManager = new DownloadsManager(this);

    m_tabWidget->addTab(m_historyManager, QSL("History"));
    m_tabWidget->addTab(m_bookmarksManager, QSL("Bookmarks"));
    m_tabWidget->addTab(m_downloadsManager, QSL("Downloads"));

    QPushButton *okButton = new QPushButton(QSL("Ok"), this);
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(new QWidget(this), 1);
    hboxLayout->addWidget(okButton);

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    vboxLayout->addWidget(m_tabWidget);
    vboxLayout->addLayout(hboxLayout);
    setLayout(vboxLayout);

    setMinimumSize(600, 400);
    setWindowTitle(QSL("%1 Manager").arg(m_tabWidget->tabText(m_tabWidget->currentIndex())));

    connect(m_tabWidget, &QTabWidget::currentChanged, this, [this](int index) { setWindowTitle(QSL("%1 Manager").arg(m_tabWidget->tabText(index))); });
    connect(okButton, &QPushButton::clicked, this, &Manager::hide);
}

DownloadsManager *Manager::downloadsManager()
{
    return m_downloadsManager;
}

void Manager::showHistoryManager()
{
    m_tabWidget->setCurrentIndex(0);
    m_historyManager->updateEntries();
    show();
    activateWindow();
}

void Manager::showBookmarksManager()
{
    m_tabWidget->setCurrentIndex(1);
    show();
    activateWindow();
}

void Manager::showDownloadsManager()
{
    m_tabWidget->setCurrentIndex(2);
    show();
    activateWindow();
}
