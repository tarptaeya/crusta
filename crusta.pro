TEMPLATE = app
TARGET = Crusta
QT += webenginewidgets sql webchannel xml


HEADERS += \
    api/qmlwindows.h \
    src/bookmarks.h \
    src/browser.h \
    src/completer.h \
    src/cookies.h \
    src/crscheme.h \
    src/database.h \
    src/downloads.h \
    src/history.h \
    src/mainwindow.h \
    src/plugin.h \
    src/plugins.h \
    src/preferences.h \
    src/searchengine.h \
    src/sidebar.h \
    src/startpage.h \
    src/tab.h \
    src/tabwidget.h \
    src/theme.h \
    src/toolbar.h \
    src/utils.h \
    src/webobject.h \
    src/webpage.h \
    src/webview.h \
    src/window.h

SOURCES += \
    api/qmlwindows.cpp \
    src/main.cpp \
    src/bookmarks.cpp \
    src/browser.cpp \
    src/completer.cpp \
    src/cookies.cpp \
    src/crscheme.cpp \
    src/database.cpp \
    src/downloads.cpp \
    src/history.cpp \
    src/mainwindow.cpp \
    src/plugin.cpp \
    src/plugins.cpp \
    src/preferences.cpp \
    src/searchengine.cpp \
    src/sidebar.cpp \
    src/startpage.cpp \
    src/tab.cpp \
    src/tabwidget.cpp \
    src/theme.cpp \
    src/toolbar.cpp \
    src/utils.cpp \
    src/webobject.cpp \
    src/webpage.cpp \
    src/webview.cpp \
    src/window.cpp

RESOURCES += \
    resources.qrc \

target.path = /usr/local/bin/

macx {
LIBS += -framework Foundation -framework Cocoa

HEADERS += \
    mac/mac_utils.h

OBJECTIVE_SOURCES += \
    mac/mac_utils.mm

ICON = mac/logos.icns
}

INSTALLS += target

