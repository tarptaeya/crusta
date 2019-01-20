set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

include_directories(
    app
    navigation
    plugins
    tabwidget
    tools
    webengine
)

set(
    SRCS ${SRCS}
    app/browserwindow.cpp
    app/database.cpp
    app/mainapplication.cpp
    app/standardpaths.cpp
    app/statusbar.cpp
    navigation/menu.cpp
    navigation/omnibar.cpp
    navigation/toolbar.cpp
    navigation/toolbarbutton.cpp
    plugins/plugins.cpp
    plugins/plugininterface.cpp
    plugins/pluginloader.cpp
    tabwidget/tab.cpp
    tabwidget/tabbar.cpp
    tabwidget/tabwidget.cpp
    tools/about.cpp
    tools/common-defs.cpp
    webengine/jsobject.cpp
    webengine/scheme.cpp
    webengine/scripts.cpp
    webengine/webpage.cpp
    webengine/webview.cpp
)

qt5_add_resources(
    SRCS
    data/data.qrc
)

add_library(CrustaPrivate SHARED ${SRCS})

target_link_libraries(CrustaPrivate
    Qt5::Widgets
    Qt5::WebEngineWidgets
    Qt5::Sql
    Qt5::WebChannel
)
