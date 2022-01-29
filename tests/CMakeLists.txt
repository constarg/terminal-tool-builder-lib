cmake_minimum_required(VERSION 3.21)
project(Test-command-tool-builder C)

set(CMAKE_C_STANDARD 17)

set(path ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(builder-test main.c)


cmake_path(GET path PARENT_PATH parent)

target_link_libraries(builder-test ${parent}/build/libtoolbuilder.a)


include_directories(../include/)

