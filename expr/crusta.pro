TEMPLATE = app
TARGET = crusta
QT += webenginewidgets

HEADERS += \
    src/browser.h \
    src/mainwindow.h \
    src/sidebar.h \
    src/tab.h \
    src/tabwidget.h \
    src/toolbar.h \
    src/utils.h \
    src/webpage.h \
    src/webview.h \
    src/window.h

SOURCES += \
    src/main.cpp \
    src/browser.cpp \
    src/mainwindow.cpp \
    src/sidebar.cpp \
    src/tab.cpp \
    src/tabwidget.cpp \
    src/toolbar.cpp \
    src/utils.cpp \
    src/webpage.cpp \
    src/webview.cpp \
    src/window.cpp

RESOURCES += \
    html/html.qrc
