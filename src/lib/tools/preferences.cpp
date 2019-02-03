#include "common-defs.h"
#include "mainapplication.h"
#include "preferences.h"

#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSizePolicy>
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

    vboxLayout->addWidget(new QWidget);
    vboxLayout->addLayout(hboxLayout);

    widget->setLayout(vboxLayout);

    widget->setMinimumSize(widget->sizeHint());
}

void Preferences::createBrowsingTab()
{
    QWidget *root = new QWidget;
    QVBoxLayout *vboxLayout = new QVBoxLayout;

    QGroupBox *webEngineSettings = new QGroupBox(QSL("QWebEngineSettings"));
    webEngineSettings->setMaximumHeight(250);
    QVBoxLayout *webEngineSettingsLayout = new QVBoxLayout;

    QScrollArea *area = new QScrollArea;
    area->setWidgetResizable(true);

    QWidget *widget = new QWidget;

    QVBoxLayout *vboxLayout1 = new QVBoxLayout;

    QCheckBox *autoLoadImages = new QCheckBox(QSL("Auto-Load Images"));
    QCheckBox *javaScriptEnabled = new QCheckBox(QSL("JavaScript Enabled"));
    QCheckBox *javaScriptCanOpenWindows = new QCheckBox(QSL("JavaScript Can Open Windows"));
    QCheckBox *javaScriptCanAccessClipboard = new QCheckBox(QSL("JavaScript Can Access Clipboard"));
    QCheckBox *linkIncludedInFocusChain = new QCheckBox(QSL("Links Included In Focus Chain"));
    QCheckBox *localStorageEnabled = new QCheckBox(QSL("Local Storage Enabled"));
    QCheckBox *localContentCanAccessRemoteUrls = new QCheckBox(QSL("Local Content Can Access Remote Urls"));
    QCheckBox *xssAuditingEnabled = new QCheckBox(QSL("XSS Auditing Enabled"));
    QCheckBox *spatialNavigationEnabled = new QCheckBox(QSL("Spatial Navigation Enabled"));
    QCheckBox *localContentCanAccessFileUrls = new QCheckBox(QSL("Local Content Can Access File Urls"));
    QCheckBox *hyperLinkAuditingEnabled = new QCheckBox(QSL("Hyperlink Auditing Enabled"));
    QCheckBox *scrollAnimatorEnabled = new QCheckBox(QSL("Scroll Animator Enabled"));
    QCheckBox *errorPageEnabled = new QCheckBox(QSL("Error Page Enabled"));
    QCheckBox *pluginsEnabled = new QCheckBox(QSL("Plugins Enabled"));
    QCheckBox *fullScreenSupportEnabled = new QCheckBox(QSL("Full Screen Support Enabled"));
    QCheckBox *screenCaptureEnabled = new QCheckBox(QSL("Screen Capture Enabled"));
    QCheckBox *webGLEnabled = new QCheckBox(QSL("WebGL Enabled"));
    QCheckBox *accelerated2dCanvasEnabled = new QCheckBox(QSL("Accelerated 2d Canvas Enabled"));
    QCheckBox *autoLoadIconsForPage = new QCheckBox(QSL("Auto-Load Icons For Page"));
    QCheckBox *touchIconsEnabled = new QCheckBox(QSL("Touch Icons Enabled"));
    QCheckBox *focusOnNavigationEnabled = new QCheckBox(QSL("Focus On Navigation Enabled"));
    QCheckBox *printElementBackgrounds = new QCheckBox(QSL("Print Element Backgrounds"));
    QCheckBox *allowRunningInsecureContent = new QCheckBox(QSL("Allow Running Insecure Content"));
    QCheckBox *allowGeolocationOnInsecureOrigins = new QCheckBox(QSL("Allow Geolocation on Insecure Origins"));
    QCheckBox *allowWindowActivationFromJavaScript = new QCheckBox(QSL("Allow Window Activation From JavaScript"));
    QCheckBox *showScrollBars = new QCheckBox(QSL("Show Scroll Bars"));
    QCheckBox *playbackRequiresUserGesture = new QCheckBox(QSL("Playback Requires User Gesture"));
    QCheckBox *javaScriptCanPaste = new QCheckBox(QSL("JavaScript Can Paste"));
    QCheckBox *webRTCPublicInterfacesOnly = new QCheckBox(QSL("WebRTC Public Interfaces Only"));
    QCheckBox *dnsPrefetchEnabled = new QCheckBox(QSL("DNS Prefetch Enabled"));

    vboxLayout1->addWidget(autoLoadImages);
    vboxLayout1->addWidget(javaScriptEnabled);
    vboxLayout1->addWidget(javaScriptCanOpenWindows);
    vboxLayout1->addWidget(javaScriptCanAccessClipboard);
    vboxLayout1->addWidget(linkIncludedInFocusChain);
    vboxLayout1->addWidget(localStorageEnabled);
    vboxLayout1->addWidget(localContentCanAccessRemoteUrls);
    vboxLayout1->addWidget(xssAuditingEnabled);
    vboxLayout1->addWidget(spatialNavigationEnabled);
    vboxLayout1->addWidget(localContentCanAccessFileUrls);
    vboxLayout1->addWidget(hyperLinkAuditingEnabled);
    vboxLayout1->addWidget(scrollAnimatorEnabled);
    vboxLayout1->addWidget(errorPageEnabled);
    vboxLayout1->addWidget(pluginsEnabled);
    vboxLayout1->addWidget(fullScreenSupportEnabled);
    vboxLayout1->addWidget(screenCaptureEnabled);
    vboxLayout1->addWidget(webGLEnabled);
    vboxLayout1->addWidget(accelerated2dCanvasEnabled);
    vboxLayout1->addWidget(autoLoadIconsForPage);
    vboxLayout1->addWidget(touchIconsEnabled);
    vboxLayout1->addWidget(focusOnNavigationEnabled);
    vboxLayout1->addWidget(printElementBackgrounds);
    vboxLayout1->addWidget(allowRunningInsecureContent);
    vboxLayout1->addWidget(allowGeolocationOnInsecureOrigins);
    vboxLayout1->addWidget(allowWindowActivationFromJavaScript);
    vboxLayout1->addWidget(showScrollBars);
    vboxLayout1->addWidget(playbackRequiresUserGesture);
    vboxLayout1->addWidget(javaScriptCanPaste);
    vboxLayout1->addWidget(webRTCPublicInterfacesOnly);
    vboxLayout1->addWidget(dnsPrefetchEnabled);

    widget->setLayout(vboxLayout1);
    area->setWidget(widget);

    webEngineSettingsLayout->addWidget(area);
    webEngineSettings->setLayout(webEngineSettingsLayout);
    vboxLayout->addWidget(webEngineSettings);

    QGridLayout *gridLayout = new QGridLayout;

    QLabel *defaultZoom = new QLabel(QSL("Default Zoom On Pages"));
    QComboBox *defaultZoomCombo = new QComboBox();
    defaultZoomCombo->setEditable(true);
    defaultZoomCombo->addItems(QStringList()
                               << QSL("25%") << QSL("50%") << QSL("75%") << QSL("100%")
                               << QSL("125%") << QSL("150%") << QSL("175%") << QSL("200%")
                               << QSL("225%") << QSL("250%") << QSL("275%") << QSL("300%"));
    gridLayout->addWidget(defaultZoom, 0, 0);
    gridLayout->addWidget(defaultZoomCombo, 0, 1);

    QLabel *userAgentLabel = new QLabel(QSL("User Agent"));
    QComboBox *userAgent = new QComboBox;
    userAgent->addItems(QStringList()
                        << QSL("Crusta") << QSL("Chrome Desktop") << QSL("Chrome Mobile")
                        << QSL("Firefox") << QSL("Firefox Mobile") << QSL("Other"));
    QLineEdit *otherUserAgent = new QLineEdit;
    otherUserAgent->setVisible(false);
    otherUserAgent->setPlaceholderText(QSL("Enter New User Agent"));

    gridLayout->addWidget(userAgentLabel, 1, 0);
    gridLayout->addWidget(userAgent, 1, 1);
    gridLayout->addWidget(otherUserAgent, 2, 1);

    vboxLayout->addLayout(gridLayout);

    QHBoxLayout *hboxLayout = new QHBoxLayout;

    QPushButton *defaults = new QPushButton(QSL("Defaults"));
    QPushButton *apply = new QPushButton(QSL("Apply"));
    QPushButton *ok = new QPushButton(QSL("Ok"));

    hboxLayout->addWidget(defaults);
    hboxLayout->addWidget(new QWidget);
    hboxLayout->addWidget(apply);
    hboxLayout->addWidget(ok);

    vboxLayout->addWidget(new QWidget);
    vboxLayout->addLayout(hboxLayout);

    root->setLayout(vboxLayout);
    addTab(root, QSL("Browsing"));

    connect(userAgent, &QComboBox::currentTextChanged, this, [otherUserAgent](const QString &text) {
        otherUserAgent->setVisible(text == QSL("Other"));
    });
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
