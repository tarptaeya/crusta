set(SRCS
    browser.cpp
    browser_schemes.cpp
    browser_window.cpp
    preferences.cpp
    resources.qrc
    tab.cpp
    webview.cpp)

add_library(crusta-private SHARED ${SRCS})
target_link_libraries(crusta-private Qt5::Widgets Qt5::WebEngineWidgets Qt5::Sql)

add_executable(crusta WIN32 MACOSX_BUNDLE main.cpp)
target_link_libraries(crusta crusta-private)

if (APPLE)
    set_target_properties(crusta PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif()
