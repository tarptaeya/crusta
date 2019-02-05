#include "common-defs.h"
#include "mainapplication.h"
#include "menu.h"
#include "omnibar.h"
#include "plugins.h"
#include "searchenginemanager.h"
#include "toolbar.h"
#include "toolbarbutton.h"

#include <QDir>
#include <QMenu>

ToolBar::ToolBar(QWidget *parent)
    : QWidget (parent)
{
    m_hboxLayout = new QHBoxLayout(this);
    m_hboxLayout->setContentsMargins(5, 5, 5, 5);

    m_omniBar = new OmniBar(this);

    m_backButton = new ToolBarButton(this);
    m_forwardButton = new ToolBarButton(this);
    m_reloadButton = new ToolBarButton(this);
    m_homeButton = new ToolBarButton(this);
    m_addEngineButton = new ToolBarButton(this);
    m_menuButton = new ToolBarButton(this);

    m_backButton->setMenu(new QMenu);
    m_forwardButton->setMenu(new QMenu);
    m_backButton->setPopupMode(ToolBarButton::DelayedPopup);
    m_forwardButton->setPopupMode(ToolBarButton::DelayedPopup);

    m_backButton->setIcon(QIcon::fromTheme(QSL("go-previous")));
    m_forwardButton->setIcon(QIcon::fromTheme(QSL("go-next")));
    m_reloadButton->setIcon(QIcon::fromTheme(QSL("view-refresh")));
    m_homeButton->setIcon(QIcon::fromTheme(QSL("go-home")));
    m_addEngineButton->setIcon(QIcon::fromTheme(QSL("list-add")));
    m_menuButton->setIcon(QIcon::fromTheme(QSL("preferences-other")));

    m_backButton->setToolTip(QSL("Go Back"));
    m_forwardButton->setToolTip(QSL("Go Forward"));
    m_reloadButton->setToolTip(QSL("Reload Page"));
    m_homeButton->setToolTip(QSL("Visit Home"));
    m_menuButton->setToolTip(QSL("Menu"));

    m_menuButton->setMenu(new Menu(this));
    m_menuButton->setPopupMode(ToolBarButton::InstantPopup);

    m_hboxLayout->addWidget(m_backButton);
    m_hboxLayout->addWidget(m_forwardButton);
    m_hboxLayout->addWidget(m_reloadButton);
    m_hboxLayout->addWidget(m_homeButton);
    m_hboxLayout->addWidget(m_omniBar, 1);
    m_hboxLayout->addWidget(m_addEngineButton);

    for (Plugin *plugin : appManager->plugins()->plugins()) {
        addPluginButton(plugin);
    }

    m_hboxLayout->addWidget(m_menuButton);

    setLayout(m_hboxLayout);

    m_addEngineButton->setVisible(false);

    connect(m_addEngineButton, &ToolBarButton::clicked, this, [this] {
        const QStringList data = m_addEngineButton->data().toStringList();
        appManager->searchEngineManager()->addSearchEngine(data);
        m_addEngineButton->setVisible(false);
    });
}

ToolBarButton *ToolBar::backButton()
{
    return m_backButton;
}

ToolBarButton *ToolBar::forwardButton()
{
    return  m_forwardButton;
}

ToolBarButton *ToolBar::reloadButton()
{
    return m_reloadButton;
}

ToolBarButton *ToolBar::homeButton()
{
    return m_homeButton;
}

OmniBar *ToolBar::omniBar()
{
    return m_omniBar;
}

ToolBarButton *ToolBar::addEngineButton()
{
    return m_addEngineButton;
}

void ToolBar::addPluginButton(Plugin *plugin)
{
    ToolBarButton *pluginButton = new ToolBarButton(this);
    pluginButton->setIcon(QIcon(QDir(plugin->path).filePath(plugin->iconFile)));
    pluginButton->setToolTip(plugin->description);
    m_hboxLayout->addWidget(pluginButton);

    QMenu *menu = new QMenu;
    pluginButton->setMenu(menu);
    pluginButton->setPopupMode(ToolBarButton::InstantPopup);

    connect(menu, &QMenu::aboutToShow, this, [menu, plugin] {
        menu->clear();
        QAction *enable = menu->addAction(QSL("Enable"));
        if (plugin->enabled) {
            enable->setText(QSL("Disable"));
        }
    });
}
