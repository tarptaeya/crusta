set(SRCS main.cpp)

add_executable(crusta ${SRCS})
target_link_libraries(crusta CrustaPrivate)
