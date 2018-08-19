#include "tablist.h"
#include "../bootstrap/appmanager.h"
#include "../bootstrap/settings.h"
#include "../utils/dimensions.h"
#include <QPropertyAnimation>

TabList::TabList(QWidget *parent)
    : QWidget(parent)
{
    appManager->settings()->beginGroup(APPEARANCE);
    bool isTabListVisible = appManager->settings()->value(QStringLiteral("tabListVisible"), false).toBool();
    m_preferedWidth = appManager->settings()->value(QStringLiteral("tabListWidth"), Dimensions::tabListWidth()).toInt();
    appManager->settings()->endGroup();

    if (!isTabListVisible) {
        hide();
    }
}

void TabList::show()
{
    resize(m_preferedWidth, height());
    QWidget::show();
}
