set(SRCS
    main.cpp)

add_executable(Crusta ${SRCS})
target_link_libraries(Crusta Qt5::Widgets)
