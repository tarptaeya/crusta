QT += core gui widgets

CONFIG += c++14

TARGET = crusta
mac: TARGET = Crusta

TEMPLATE += app

SOURCES += \
    browser.cpp \
    main.cpp

HEADERS += \
    browser.h
