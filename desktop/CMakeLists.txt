set(SRCS
    browser.cpp
    browser_window.cpp
    main.cpp
    resources.qrc
    tab.cpp
    webview.cpp)

add_executable(Crusta ${SRCS})
target_link_libraries(Crusta Qt5::Widgets Qt5::WebEngineWidgets)
