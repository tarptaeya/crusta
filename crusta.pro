QT += core gui widgets webengine webenginewidgets

DEFINES += QT_DEPRECIATED_WARNINGS

SOURCES += \
    src/main.cpp \
    src/bootstrap/appmanager.cpp \
    src/bootstrap/settings.cpp \
    src/tabview/tabwidget.cpp \
    src/tabview/tab.cpp \
    src/tabview/tabbar.cpp \
    src/tabview/stackedwidget.cpp \
    src/tabview/tabbarbutton.cpp \
    src/tabview/omnibar/omnibar.cpp \
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
    src/utils/layouts.cpp \

HEADERS += \
    src/bootstrap/appmanager.h \
    src/bootstrap/settings.h \
    src/tabview/tabwidget.h \
    src/tabview/tab.h \
    src/tabview/tabbar.h \
    src/tabview/stackedwidget.h \
    src/tabview/tabbarbutton.h \
    src/tabview/omnibar/omnibar.h \
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
    src/utils/layouts.h \

RESOURCES += \
    assets/assets.qrc \

TEMPLATE = app
TARGET = crusta

unix:!macx {
target.path = /usr/local/bin
desktop.path = /usr/share/applications
desktop.files += crusta.desktop
INSTALLS += target desktop
}
