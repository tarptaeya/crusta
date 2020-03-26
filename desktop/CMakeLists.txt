set(SRCS
    bookmarks.cpp
    browser.cpp
    browser_schemes.cpp
    browser_window.cpp
    history.cpp
    main.cpp
    resources.qrc
    tab.cpp
    webview.cpp)

add_executable(crusta WIN32 MACOSX_BUNDLE ${SRCS})
target_link_libraries(crusta Qt5::Widgets Qt5::WebEngineWidgets Qt5::Sql)

if (APPLE)
    set_target_properties(crusta PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif()
