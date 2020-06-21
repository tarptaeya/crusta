QT += core gui widgets webengine webenginewidgets webchannel

CONFIG += c++14

TARGET = crusta
mac: TARGET = Crusta

TEMPLATE += app

SOURCES += \
    addressbar.cpp \
    browser.cpp \
    main.cpp \
    tab.cpp \
    tabwidget.cpp \
    toolbar.cpp \
    webpage.cpp \
    webview.cpp \
    window.cpp

HEADERS += \
    addressbar.h \
    browser.h \
    tab.h \
    tabwidget.h \
    toolbar.h \
    webpage.h \
    webview.h \
    window.h

TRANSLATIONS += \
    ../translations/Crusta_hi_IN.ts

RESOURCES += \
    ../assets/assets.qrc
