#include "omnibar.h"
#include <QUrl>

OmniBar::OmniBar(QWidget *parent)
    : QLineEdit(parent)
{
    int height = parent->height() - 10;
    setFixedHeight(height);
    setAttribute(Qt::WA_MacShowFocusRect, 0);
    setTextMargins(10, 4, 10, 4);
}

void OmniBar::setUrl(const QUrl &url)
{
    setText(url.toString());
    setCursorPosition(0);
}
