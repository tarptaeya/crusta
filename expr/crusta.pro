QT += widgets core webengine webenginewidgets sql

HEADERS += \
    src/browser.h \
    src/sidebar.h \
    src/tab.h \
    src/tabwidget.h \
    src/toolbar.h \
    src/webview.h \
    src/window.h

SOURCES += \
    src/main.cpp \
    src/browser.cpp \
    src/sidebar.cpp \
    src/tab.cpp \
    src/tabwidget.cpp \
    src/toolbar.cpp \
    src/webview.cpp \
    src/window.cpp

TARGET = crusta
