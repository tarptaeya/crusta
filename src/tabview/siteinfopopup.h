#pragma once

#include "toolbarpopup.h"
#include <QUrl>
#include <QLabel>
#include <QVBoxLayout>
#include <QAction>

class SiteInfoPopup : public ToolBarPopup
{
public:
    explicit SiteInfoPopup(QWidget *parent = nullptr);
    void setParentAction(QAction *action);
    void setUrl(const QUrl &url);
    void show();
private:
    QAction *m_parentAction = nullptr;
    QLabel *m_typeLabel = nullptr;
};
