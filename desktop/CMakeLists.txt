set(SRCS
    browser.cpp
    browser_window.cpp
    main.cpp
    resources.qrc)

add_executable(Crusta ${SRCS})
target_link_libraries(Crusta Qt5::Widgets)
