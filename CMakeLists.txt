cmake_minimum_required(VERSION 3.7.0)

project(crusta-browser)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

find_package(Qt5 COMPONENTS Widgets WebEngine WebEngineWidgets Sql Test REQUIRED)

enable_testing()

add_subdirectory(desktop)
add_subdirectory(tests)
