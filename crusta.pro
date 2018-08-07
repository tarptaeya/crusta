QT += core gui widgets webengine

DEFINES += QT_DEPRECIATED_WARNINGS

SOURCES += \
    src/main.cpp \
    src/bootstrap/appmanager.cpp \
    src/tabview/tabview.cpp \
    src/window/window.cpp \
    src/window/centralwidget.cpp \
    src/window/menubar.cpp \
    src/window/statusbar.cpp \
    src/webview/webview.cpp \
    src/webview/webpage.cpp \
    src/sidebar/sidebar.cpp \
    src/sidebar/sidebarbutton.cpp \
    src/sidebar/sidebarpopup.cpp \
    src/utils/dimensions.cpp \
    src/utils/strings.cpp \

HEADERS += \
    src/bootstrap/appmanager.h \
    src/tabview/tabview.h \
    src/window/window.h \
    src/window/centralwidget.h \
    src/window/menubar.h \
    src/window/statusbar.h \
    src/webview/webview.h \
    src/webview/webpage.h \
    src/sidebar/sidebar.h \
    src/sidebar/sidebarbutton.h \
    src/sidebar/sidebarpopup.h \
    src/utils/dimensions.h \
    src/utils/strings.h \

TEMPLATE = app
TARGET = crusta

unix:!macx {
target.path = /usr/local/bin
desktop.path = /usr/share/applications
desktop.files += crusta.desktop
INSTALLS += target desktop
}
