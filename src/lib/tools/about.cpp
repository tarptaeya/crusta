#include "about.h"
#include "common-defs.h"
#include "mainapplication.h"

#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>

About::About(QWidget *parent)
    : QDialog (parent)
{
    QVBoxLayout *vboxLayout = new QVBoxLayout(this);

    QLabel *appVersion = new QLabel(QSL("<b>Crusta version %1</b>").arg(CRUSTA_VERSION_STR));

    QLabel *qtVersion = new QLabel(QSL("<b>Qt version %1</b>").arg(QT_VERSION_STR));

    QLabel *copyright = new QLabel(QSL("&copy; Anmol Gautam<br><a href='mailto:anmol@crusta.io'>anmol@crusta.io</a>"));
    copyright->setTextFormat(Qt::RichText);
    copyright->setOpenExternalLinks(true);

    appVersion->setAlignment(Qt::AlignCenter);
    qtVersion->setAlignment(Qt::AlignCenter);
    copyright->setAlignment(Qt::AlignCenter);

    vboxLayout->addWidget(appVersion);
    vboxLayout->addWidget(qtVersion);
    vboxLayout->addWidget(copyright);

    setLayout(vboxLayout);
    setWindowTitle(QSL("About Crusta"));
    setMinimumWidth(300);
}
