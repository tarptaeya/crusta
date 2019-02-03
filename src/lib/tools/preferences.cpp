#include "common-defs.h"
#include "mainapplication.h"
#include "preferences.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

Preferences::Preferences(QWidget *parent)
    : QTabWidget (parent)
{
    createGeneralTab();
    createBrowsingTab();
    createSearchEngineTab();
    createOthersTab();
}

void Preferences::show()
{
    activateWindow();
    QTabWidget::show();
}

void Preferences::createGeneralTab()
{
    QWidget *widget = new QWidget;
    addTab(widget, QSL("General"));

    QVBoxLayout *vboxLayout = new QVBoxLayout;

    QGroupBox *launch = new QGroupBox(QSL("On Launch"));
    QGridLayout *gridLayout1 = new QGridLayout;

    QLabel *homePage = new QLabel(QSL("Home Page"));
    QLineEdit *homePageUrl = new QLineEdit;
    QPushButton *homePageCurrent = new QPushButton(QSL("Use Current"));
    QLabel *newTab = new QLabel(QSL("New Tab"));
    QLineEdit *newTabUrl = new QLineEdit;
    QPushButton *newTabCurrent = new QPushButton(QSL("Use Current"));

    gridLayout1->addWidget(homePage, 0, 0);
    gridLayout1->addWidget(homePageUrl, 0, 1);
    gridLayout1->addWidget(homePageCurrent, 0, 2);
    gridLayout1->addWidget(newTab, 1, 0);
    gridLayout1->addWidget(newTabUrl, 1, 1);
    gridLayout1->addWidget(newTabCurrent, 1, 2);

    launch->setLayout(gridLayout1);

    QGroupBox *profile = new QGroupBox(QSL("Profile"));
    QGridLayout *gridLayout2 = new QGridLayout;

    QLabel *activeProfileLabel = new QLabel(QSL("Active Profile"));
    QLabel *activeProfileName = new QLabel(appManager->webEngineProfile()->storageName());
    QLabel *startUpProfile = new QLabel(QSL("Startup Profile"));
    QComboBox *profileCombo = new QComboBox;
    QPushButton *newProfile = new QPushButton(QSL("New"));
    QPushButton *delProfile = new QPushButton(QSL("Remove"));

    gridLayout2->addWidget(activeProfileLabel, 0, 0);
    gridLayout2->addWidget(activeProfileName, 0, 1);
    gridLayout2->addWidget(startUpProfile, 1, 0);
    gridLayout2->addWidget(profileCombo, 1, 1);

    QHBoxLayout *hboxLayout2 = new QHBoxLayout;
    hboxLayout2->addWidget(newProfile);
    hboxLayout2->addWidget(delProfile);
    gridLayout2->addLayout(hboxLayout2, 2, 1);

    profile->setLayout(gridLayout2);

    vboxLayout->addWidget(launch);
    vboxLayout->addWidget(profile);

    QHBoxLayout *hboxLayout = new QHBoxLayout;

    QPushButton *defaults = new QPushButton(QSL("Defaults"));
    QPushButton *apply = new QPushButton(QSL("Apply"));
    QPushButton *ok = new QPushButton(QSL("Ok"));

    hboxLayout->addWidget(defaults);
    hboxLayout->addWidget(new QWidget);
    hboxLayout->addWidget(apply);
    hboxLayout->addWidget(ok);

    vboxLayout->addLayout(hboxLayout);

    widget->setLayout(vboxLayout);

    widget->setMinimumSize(widget->sizeHint());
}

void Preferences::createBrowsingTab()
{
    QWidget *widget = new QWidget;
    addTab(widget, QSL("Browsing"));
}

void Preferences::createSearchEngineTab()
{
    QWidget *widget = new QWidget;
    addTab(widget, QSL("Search Engine"));
}

void Preferences::createOthersTab()
{
    QWidget *widget = new QWidget;
    addTab(widget, QSL("Others"));
}
