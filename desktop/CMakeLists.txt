set(SRCS
    browser.cpp
    browser_schemes.cpp
    browser_window.cpp
    main.cpp
    preferences.cpp
    resources.qrc
    tab.cpp
    webview.cpp)

add_executable(Crusta WIN32 MACOSX_BUNDLE ${SRCS})
target_link_libraries(Crusta Qt5::Widgets Qt5::WebEngineWidgets Qt5::Sql)

if (APPLE)
    set_target_properties(Crusta PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif()
