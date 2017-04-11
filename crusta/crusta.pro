#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T20:08:06
#
#-------------------------------------------------

QT       += core gui
QT       +=webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crusta
TEMPLATE = app


SOURCES += main.cpp\
    fullscreennotifier.cpp \
    searchlineedit.cpp \
    addresslineedit.cpp \
    presentationmodenotifier.cpp \
    webview.cpp \
    tabwindow.cpp \
    mainview.cpp \
    timenotifier.cpp \
    popup.cpp \
    manager.cpp \
    historymanager.cpp

HEADERS  += \
    fullscreennotifier.h \
    searchlineedit.h \
    addresslineedit.h \
    presentationmodenotifier.h \
    webview.h \
    tabwindow.h \
    mainview.h \
    timenotifier.h \
    popup.h \
    manager.h \
    historymanager.h

RESOURCES += \
    resource.qrc
