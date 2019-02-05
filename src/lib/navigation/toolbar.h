#ifndef CR_TOOL_BAR_H
#define CR_TOOL_BAR_H

#include <QHBoxLayout>
#include <QWidget>

struct Plugin;

class OmniBar;
class ToolBarButton;

class ToolBar : public QWidget
{
public:
    explicit ToolBar(QWidget *parent = nullptr);

    ToolBarButton *backButton();
    ToolBarButton *forwardButton();
    ToolBarButton *reloadButton();
    ToolBarButton *homeButton();
    OmniBar *omniBar();
    ToolBarButton *addEngineButton();

private:
    void addPluginButton(Plugin *plugin);

    QHBoxLayout *m_hboxLayout = nullptr;

    OmniBar *m_omniBar = nullptr;

    ToolBarButton *m_backButton = nullptr;
    ToolBarButton *m_forwardButton = nullptr;
    ToolBarButton *m_reloadButton = nullptr;
    ToolBarButton *m_homeButton = nullptr;
    ToolBarButton *m_addEngineButton = nullptr;
    ToolBarButton *m_menuButton = nullptr;
};

#endif
