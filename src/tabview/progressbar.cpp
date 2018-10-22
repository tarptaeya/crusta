#include "progressbar.h"

ProgressBar::ProgressBar(QWidget *parent)
    : QProgressBar(parent)
{
    setObjectName(QStringLiteral("progress-bar"));
}
