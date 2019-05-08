TEMPLATE = app
TARGET = crusta
QT += webenginewidgets

HEADERS += \
    src/browser.h \
    src/history.h \
    src/mainwindow.h \
    src/sidebar.h \
    src/tab.h \
    src/tabwidget.h \
    src/theme.h \
    src/toolbar.h \
    src/utils.h \
    src/webpage.h \
    src/webview.h \
    src/window.h

SOURCES += \
    src/main.cpp \
    src/browser.cpp \
    src/history.cpp \
    src/mainwindow.cpp \
    src/sidebar.cpp \
    src/tab.cpp \
    src/tabwidget.cpp \
    src/theme.cpp \
    src/toolbar.cpp \
    src/utils.mm \
    src/webpage.cpp \
    src/webview.cpp \
    src/window.cpp

RESOURCES += \
    resources.qrc \

macx {
    LIBS += -framework Foundation -framework Cocoa
}
