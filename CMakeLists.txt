cmake_minimum_required(VERSION 3.1)

project(Crusta VERSION 2.0)

set(QT_MIN_VERSION "5.11.0")
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Widgets Sql WebEngine WebEngineWidgets WebChannel)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/lib/app
    ${CMAKE_SOURCE_DIR}/src/lib/bookmarks
    ${CMAKE_SOURCE_DIR}/src/lib/cookies
    ${CMAKE_SOURCE_DIR}/src/lib/downloads
    ${CMAKE_SOURCE_DIR}/src/lib/history
    ${CMAKE_SOURCE_DIR}/src/lib/plugins
    ${CMAKE_SOURCE_DIR}/src/lib/session
    ${CMAKE_SOURCE_DIR}/src/lib/tabwidget
    ${CMAKE_SOURCE_DIR}/src/lib/tools
    ${CMAKE_SOURCE_DIR}/src/lib/webengine
)

add_subdirectory(src)
