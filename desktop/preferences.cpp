#include "preferences.h"
#include <QVBoxLayout>
#include <QStyleFactory>

void PreferencesWindow::setup_toolbar()
{
    m_toolbar = new QToolBar;
    m_toolbar->setMovable(false);
    addToolBar(m_toolbar);

    m_toolbar->addAction(QStringLiteral("General"));
    m_toolbar->addAction(QStringLiteral("History"));
    m_toolbar->addAction(QStringLiteral("Bookmarks"));
    m_toolbar->addAction(QStringLiteral("Cookies"));
    m_toolbar->addAction(QStringLiteral("Search"));

#ifdef Q_OS_MACOS
    setUnifiedTitleAndToolBarOnMac(true);
    m_toolbar->setStyle(QStyleFactory::create(QStringLiteral("macintosh")));
#endif
}

PreferencesWindow::PreferencesWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setup_toolbar();
}
