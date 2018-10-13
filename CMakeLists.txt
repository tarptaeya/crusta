cmake_minimum_required(VERSION 3.1)

project(Crusta VERSION 2.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/build)

set(QT_MIN_VERSION "5.9.0")
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Widgets Network Sql WebEngine WebEngineWidgets WebChannel)

add_subdirectory(src)