set(SRCS
    adblock.cpp
    bookmarks.cpp
    browser.cpp
    browser_schemes.cpp
    browser_window.cpp
    history.cpp
    plugins.cpp
    request_interceptor.cpp
    resources.qrc
    search_engine.cpp
    tab.cpp
    webview.cpp)

add_library(crusta-private SHARED ${SRCS})
target_link_libraries(crusta-private Qt5::QuickWidgets Qt5::Sql Qt5::Widgets Qt5::WebEngineWidgets)

add_executable(crusta WIN32 MACOSX_BUNDLE main.cpp)
target_link_libraries(crusta crusta-private)

if (APPLE)
    set_target_properties(crusta PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif()
