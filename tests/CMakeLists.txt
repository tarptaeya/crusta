include_directories(../desktop)

add_executable(adblock test_adblock.cpp)
add_test(NAME adblock COMMAND adblock)
target_link_libraries(adblock PRIVATE crusta-private Qt5::Test)
