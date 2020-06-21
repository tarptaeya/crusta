QT += core gui widgets

CONFIG += c++14

TARGET = crusta
mac: TARGET = Crusta

TEMPLATE += app

SOURCES += \
    browser.cpp \
    main.cpp \
    tab.cpp \
    tabwidget.cpp \
    toolbar.cpp \
    window.cpp

HEADERS += \
    browser.h \
    tab.h \
    tabwidget.h \
    toolbar.h \
    window.h
