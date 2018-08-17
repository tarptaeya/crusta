#include "omnibar.h"
#include "../../utils/dimensions.h"
#include <QUrl>

OmniBar::OmniBar(QWidget *parent)
    : QLineEdit(parent)
{
    int height = parent->height() - Dimensions::onmibarHeightOffsetFromParent();
    setFixedHeight(height);
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setTextMargins(Dimensions::omnibarMargins());
}

void OmniBar::setUrl(const QUrl &url)
{
    setText(url.toString());
    setCursorPosition(0);
}
